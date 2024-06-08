#include "image.hpp"

namespace ML
{

Image::~Image() {}

void ImagePNG::read(const std::string& path){
    std::ifstream fp(path.c_str(), std::ios::binary);

    if(!fp){
        std::cout<<"Error: cannot open image file";
        throw std::runtime_error ("Error: cannot open image file");
    }
    
    _freeData();

    _pngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    _infoPtr = png_create_info_struct(_pngPtr);
    png_set_read_fn(_pngPtr, &fp, _userReadData);

    png_read_info(_pngPtr, _infoPtr);
    _w = png_get_image_width(_pngPtr, _infoPtr);
    _h = png_get_image_height(_pngPtr, _infoPtr);
    _channels = png_get_channels(_pngPtr, _infoPtr);

	unsigned int rowbytes = png_get_rowbytes(_pngPtr, _infoPtr);
    _rowPointers = (png_bytep*)malloc(sizeof(png_bytep) * _h);
    for(int y = 0; y < _h; y++) {
        _rowPointers[y] = (png_byte*)malloc(rowbytes);
    }

    png_read_image(_pngPtr, _rowPointers);
}

void ImagePNG::write(const std::string& path){
    std::ofstream fp(path.c_str(), std::ios::binary);
    
    if(!fp){
        std::cout<<"Error: cannot write image file";
        throw std::runtime_error ("Error: cannot write image file");
    }

    png_struct* png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_set_write_fn(png, &fp, _userWriteData, _userFlushData);

    if (setjmp(png_jmpbuf(png))) // this is some routine for errors?
    {
        printf("setjmp failed\n");
    }

    png_write_info(png, _infoPtr);
    png_write_image(png, _rowPointers);
    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &_infoPtr);
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

void ImagePNG::_userReadData(png_structp pngPtr, png_bytep data, png_size_t length){
    std::istream *from = reinterpret_cast< std::istream * >(png_get_io_ptr(pngPtr));
	assert(from);
	if (!from->read(reinterpret_cast< char * >(data), length)) {
		png_error(pngPtr, "Error reading.");
	}
}

void ImagePNG::_userWriteData(png_structp pngPtr, png_bytep data, png_size_t length) {
	std::ostream *to = reinterpret_cast< std::ostream * >(png_get_io_ptr(pngPtr));
	assert(to);
	if (!to->write(reinterpret_cast< char * >(data), length)) {
		png_error(pngPtr, "Error writing.");
	}
}

void ImagePNG::_userFlushData(png_structp pngPtr) {
	std::ostream *to = reinterpret_cast< std::ostream * >(png_get_io_ptr(pngPtr));
	assert(to);
	if (!to->flush()) {
		png_error(pngPtr, "Error flushing.");
	}
}

ImagePNG::~ImagePNG(){
    _freeData();
}

Image *ImageCreator::createPNG(const std::string &path) const
{
    Image* img = new ImagePNG;
    img->read(path);
    return img;
}

Image *ImageCreator::createImage(const std::string &path) const
{
    auto extension = std::filesystem::path(path).extension();

    if(extension == ".png") return createPNG(path);

    return nullptr;
}

} // namespace ML
