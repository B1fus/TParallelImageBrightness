#include <iostream>
#include <vector>
#include <string>
#include <png.h>

namespace ML
{
    
class Image{
public:
    virtual void read(const std::string& path) = 0;
    virtual void write(const std::string& path) = 0;
    //virtual void setPix(size_t x, size_t y, std::vector<uint8_t> pix) = 0;
    //virtual std::vector<uint8_t> getPix(size_t x, size_t y) const = 0;
   
    virtual void addPix(size_t x, size_t y, std::vector<uint8_t> addVals) = 0;
    virtual size_t getWidth() const = 0;
    virtual size_t getHeight() const = 0;
    virtual uint8_t getChannels() const = 0;
    virtual ~Image();
};

class ImagePNG : public Image{
    png_infop _infoPtr; 
    png_struct* _pngPtr; 
    size_t _w, _h;
    png_bytepp _rowPointers;
    uint8_t _channels;

    void _freeData();
    //return true if valid position of pixel
    bool _checkPixelPos(size_t x, size_t y) const;
public:
    void read(const std::string& path) override;
    void write(const std::string& path) override;
    //void setPix(size_t x, size_t y, std::vector<uint8_t> pix) override;
    
    void addPix(size_t x, size_t y, std::vector<uint8_t> addVals) override;
    size_t getWidth() const override;
    size_t getHeight() const override;
    uint8_t getChannels() const override;
    ~ImagePNG();
};

} //namespace ML