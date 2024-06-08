#include "image.hpp"

namespace ML
{

Image::~Image() {}

void ImagePNG::read(const std::string& path){
    FILE *fp;

    errno_t err = fopen_s(&fp, path.c_str(), "rb");
    if(err != 0){
        std::cout<<strerror(err);
        throw std::runtime_error ("Error: cannot open image file");
    }
    
    _freeData();

    _pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    _infoPtr = png_create_info_struct(_pngPtr);

    png_init_io(_pngPtr, fp);
    png_read_png(_pngPtr, _infoPtr, PNG_TRANSFORM_IDENTITY, NULL);
    _rowPointers = png_get_rows(_pngPtr, _infoPtr);

    _w = png_get_image_width(_pngPtr, _infoPtr);
    _h = png_get_image_height(_pngPtr, _infoPtr);
    _channels = png_get_channels(_pngPtr, _infoPtr);
    fclose(fp);
}

void ImagePNG::write(const std::string& path){
    FILE *fp;

    std::cout<<(int)_channels<<" ";

    errno_t err = fopen_s(&fp, path.c_str(), "wb");
    if(err != 0){
        std::cout<<strerror(err);
        throw std::runtime_error ("Error: cannot open writing path");
    }

    png_struct* png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (setjmp(png_jmpbuf(png))) // this is some routine for errors?
    {
        printf("setjmp failed\n");
    }

    png_init_io(png, fp);
    png_write_info(png, _infoPtr);
    png_write_image(png, _rowPointers);
    png_write_end(png, NULL);
}

// void ImagePNG::setPix(size_t x, size_t y, std::vector<uint8_t> pix){
//     if(!_checkPixelPos(x, y)) return;
//     if(pix.size() < _channels) return;

//     for(size_t i = 0; i<_channels; i++)
//         _rowPointers[y][x*_channels + i] = pix[i];
// }

void ImagePNG::addPix(size_t x, size_t y, std::vector<uint8_t> addVals)
{
    for(size_t i = 0; i<_channels; i++){
        if(_rowPointers[y][x*_channels + i] <= UINT8_MAX - addVals[i])
            _rowPointers[y][x*_channels + i] += addVals[i];
        else _rowPointers[y][x*_channels + i] = UINT8_MAX;
    }
}

size_t ImagePNG::getWidth() const
{
    return _w;
}

size_t ImagePNG::getHeight() const{
    return _h;
}

uint8_t ImagePNG::getChannels() const{
    return _channels;
}

void ImagePNG::_freeData(){
    if(_pngPtr != nullptr){
        png_free_data(_pngPtr, _infoPtr, PNG_FREE_ALL, -1);
    }
}

inline bool ImagePNG::_checkPixelPos(size_t x, size_t y) const
{
    if(x>=_w || y>=_h) return false;
    return true;
}

ImagePNG::~ImagePNG(){
    _freeData();
}

} // namespace ML
