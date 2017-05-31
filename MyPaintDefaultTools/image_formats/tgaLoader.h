#ifndef MP_TGA_LOADER_H
#define MP_TGA_LOADER_H
#include <image_formats/loader.h>

namespace mp
{

class CTGALoader : public CImageLoader
{
public:
    CTGALoader();
    ~CTGALoader() = default;

    void    Write(const std::string &path, const CImage *img) override;
    CImage* Read(const std::string &path) override;

private:
    struct STGAFileHeader
    {
        unsigned char header[12];
    };

    struct STGA
    {
        unsigned char header[6];
    };
};

}

#endif
