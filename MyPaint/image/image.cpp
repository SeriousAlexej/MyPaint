#include "image.h"
#include <glm/common.hpp>
#include <cstring>

namespace mp
{

CImage::CImage(const glm::uvec2& imgSize)
{
    imageSize = imgSize;
    //do not allow images with 0-dimensions
    imageSize.x = glm::max(1u, imageSize.x);
    imageSize.y = glm::max(1u, imageSize.y);

    AllocatePixels();

    //default image is white
    std::memset(pixels.get(), 255, CountUsedBytes());
}

CImage::CImage(const CImage& other)
{
    imageSize = other.imageSize;
    AllocatePixels();
    std::memcpy(pixels.get(), other.pixels.get(), CountUsedBytes());
}

CImage::CImage(CImage&& other)
{
    imageSize = other.imageSize;
    pixels = std::move(other.pixels);
    other.imageSize = glm::uvec2(0u, 0u);
}

CImage& CImage::operator=(const CImage& other)
{
    if(this != &other)
    {
        //placement new to call copy constructor
        new (this) CImage(other);
    }
    return *this;
}

CImage& CImage::operator=(CImage&& other)
{
    if(this != &other)
    {
        //call move constructor
        new (this) CImage(other);
    }
    return *this;
}

bool CImage::operator==(const CImage& other) const
{
    return CountUsedBytes() == other.CountUsedBytes() &&
           std::memcmp(pixels.get(), other.pixels.get(), CountUsedBytes()) == 0;
}

void CImage::CopyImageDataFrom(const CImage &other)
{
    if(this == &other)
    {
        return;
    }

    CopyRectFrom(other, glm::ivec2(0, 0), imageSize);
}

void CImage::CopyRectFrom(const CImage &other, glm::ivec2 posMin, glm::ivec2 posMax)
{
    if(this == &other)
    {
        return;
    }

    int maxX = glm::min(imageSize.x, other.imageSize.x) - 1;
    int maxY = glm::min(imageSize.y, other.imageSize.y) - 1;

    posMin.x = glm::clamp(posMin.x, 0, maxX);
    posMax.x = glm::clamp(posMax.x, posMin.x, maxX);
    posMin.y = glm::clamp(posMin.y, 0, maxY);
    posMax.y = glm::clamp(posMax.y, posMin.y, maxY);

    const std::size_t bytesInRow = (posMax.x - posMin.x + 1) * 3;

    for(int y = posMin.y; y <= posMax.y; y++)
    {
        std::memcpy(GetPixelPtr(glm::uvec2(posMin.x, y)), other.GetPixelPtr(glm::uvec2(posMin.x, y)), bytesInRow);
    }
}

glm::uvec2 CImage::GetSize() const
{
    return imageSize;
}

void CImage::SetPixelColor(const glm::uvec2& pos, const glm::uvec3& color)
{
    unsigned char* pixel = GetPixelPtr(pos);
    if(!pixel)
    {
        return;
    }
    pixel[0] = (unsigned char)color.r;
    pixel[1] = (unsigned char)color.g;
    pixel[2] = (unsigned char)color.b;
}

glm::uvec3 CImage::GetPixelColor(const glm::uvec2& pos) const
{
    unsigned char* pixel = GetPixelPtr(pos);
    if(!pixel)
    {
        return glm::uvec3(0u, 0u, 0u);
    }
    return glm::uvec3(pixel[0], pixel[1], pixel[2]);
}

unsigned char* CImage::GetPixelPtr(const glm::uvec2& pos) const
{
    if(!CoordinateWithinImage(pos))
    {
        return nullptr;
    }
    return &pixels[(imageSize.x * pos.y + pos.x) * 3];
}

const CImage::pixelsPtr& CImage::GetRawPixels() const
{
    return pixels;
}

void CImage::AllocatePixels()
{
    pixels = pixelsPtr(new unsigned char[CountUsedBytes()]);
}

std::size_t CImage::CountUsedBytes() const
{
    //x * y * 3 bytes total, 3 bytes for one pixel, for RGB channels
    return imageSize.x * imageSize.y * 3;
}

bool CImage::CoordinateWithinImage(const glm::uvec2& pos) const
{
    return pos.x < imageSize.x &&
           pos.y < imageSize.y;
}

}
