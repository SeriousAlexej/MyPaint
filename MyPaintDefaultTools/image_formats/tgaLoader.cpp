#include <cstdio>
#include <cstring>
#include "tgaLoader.h"
#include <image/image.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

static const unsigned char uncompressedTGAHeader[12] = {0,0,2,0,0,0,0,0,0,0,0,0};

namespace mp
{

CTGALoader::CTGALoader()
{
    format = "tga";
}

void CTGALoader::Write(const std::string &path, const CImage *img)
{
    if(!img)
    {
        return;
    }

    std::FILE* file = std::fopen(path.c_str(), "wb");
    if(!file)
    {
        return;
    }

    STGA tga;
    glm::uvec2 imgSize = img->GetSize();
    tga.header[0] = (imgSize.x & 0x00ff);
    tga.header[1] = (imgSize.x & 0xff00) >> 8;
    tga.header[2] = (imgSize.y & 0x00ff);
    tga.header[3] = (imgSize.y & 0xff00) >> 8;
    tga.header[4] = 24;
    tga.header[5] = 0;

    std::fwrite(uncompressedTGAHeader, sizeof(STGAFileHeader), 1, file);
    std::fwrite(&tga, sizeof(STGA), 1, file);

    for(int y = imgSize.y - 1; y >= 0; y--)
    for(unsigned x = 0; x < imgSize.x; x++)
    {
        glm::uvec3 col = const_cast<CImage*>(img)->GetPixelColor(glm::uvec2(x, y));
        unsigned char colors[3] = { (unsigned char)col.b,
                                    (unsigned char)col.g,
                                    (unsigned char)col.r };
        std::fwrite(colors, sizeof(unsigned char), 3, file);
    }

    std::fclose(file);
}

CImage* CTGALoader::Read(const std::string &path)
{
    std::FILE* file = std::fopen(path.c_str(), "rb");

    if(!file)
    {
        return nullptr;
    }

    STGAFileHeader tgaHeader;

    if(std::fread(&tgaHeader, sizeof(STGAFileHeader), 1, file) != 1)
    {
        std::fclose(file);
        return nullptr;
    }

    if(std::memcmp(uncompressedTGAHeader, &tgaHeader, sizeof(STGAFileHeader)) != 0)
    {
        std::fclose(file);
        return nullptr;
    }

    STGA tga;

    if(std::fread(tga.header, sizeof(tga.header), 1, file) == 0)
    {
        std::fclose(file);
        return nullptr;
    }

    glm::uvec2 imgSize;

    imgSize.x = ((int)tga.header[1] << 8) + tga.header[0];
    imgSize.y = ((int)tga.header[3] << 8) + tga.header[2];
    unsigned bpp = tga.header[4];

    if(imgSize.x == 0 || imgSize.y == 0 || bpp != 24)
    {
        std::fclose(file);
        return nullptr;
    }

    CImage* img = new CImage(imgSize);

    if(!img || std::fread(const_cast<unsigned char*>(img->GetRawPixels().get()), 1, img->CountUsedBytes(), file) != img->CountUsedBytes())
    {
        delete img;
        std::fclose(file);
        return nullptr;
    }

    unsigned char* imgData = const_cast<unsigned char*>(img->GetRawPixels().get());

    for(unsigned i = 0, sz = img->CountUsedBytes(); i < sz; i += 3)
    {
        std::swap(imgData[i], imgData[i+2]);
    }

    for(unsigned x = 0; x < imgSize.x; x++)
    for(unsigned y = 0; y < imgSize.y / 2; y++)
    {
        glm::uvec3 col1 = img->GetPixelColor(glm::uvec2(x, y));
        glm::uvec3 col2 = img->GetPixelColor(glm::uvec2(x, imgSize.y - y - 1));
        img->SetPixelColor(glm::uvec2(x, y), col2);
        img->SetPixelColor(glm::uvec2(x, imgSize.y - y - 1), col1);
    }

    std::fclose(file);

    return img;
}

}
