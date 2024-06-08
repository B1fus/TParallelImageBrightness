#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <fstream>
#include <cassert>
#include <png.h>

namespace ML
{
    
class Image{
public:
    virtual void read(const std::string& path) = 0;
    virtual void write(const std::string& path) = 0;
    virtual void addPix(size_t x, size_t y, std::vector<uint8_t> addVals) = 0;
    virtual size_t getWidth() const = 0;
    virtual size_t getHeight() const = 0;
    virtual uint8_t getChannels() const = 0;
    virtual ~Image();
};

class ImagePNG : public Image{
    png_infop _infoPtr; 
    png_structp _pngPtr = nullptr; 
    size_t _w, _h;
    png_bytepp _rowPointers;
    uint8_t _channels;

    void _freeData();
    //return true if valid position of pixel
    bool _checkPixelPos(size_t x, size_t y) const;
    static void _userReadData(png_structp pngPtr, png_bytep data, png_size_t length);
    static void _userWriteData(png_structp pngPtr, png_bytep data, png_size_t length);
    static void _userFlushData(png_structp pngPtr) ;
public:
    void read(const std::string& path) override;
    void write(const std::string& path) override;   
    //add rgb value to pixel on image 
    void addPix(size_t x, size_t y, std::vector<uint8_t> addVals) override;
    size_t getWidth() const override;
    size_t getHeight() const override;
    uint8_t getChannels() const override;
    ~ImagePNG();
};



class ImageCreator{
public:
    Image* createPNG(const std::string& path) const;
    //creates Image depends on file extension
    Image* createImage(const std::string& path) const;
};

} //namespace ML
