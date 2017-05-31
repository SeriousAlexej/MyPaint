#ifndef MY_PAINT_IMAGE_H
#define MY_PAINT_IMAGE_H
#include <memory>
#include <glm/vec2.hpp>
#include "../mypaint_api.h"

namespace mp
{

/**
 * CImage class contains raw image data
 * and provides interface for manipulating it
 */
class MYPAINT_API CImage
{
public:
    typedef std::unique_ptr<unsigned char[]> pixelsPtr;

    explicit CImage(const glm::uvec2& imgSize);
    CImage(const CImage& other);
    CImage(CImage&& other);
    ~CImage() = default;

    CImage& operator=(const CImage& other);
    CImage& operator=(CImage&& other);

    bool operator==(const CImage& other) const;

    glm::uvec2  GetSize() const;

    void        CopyImageDataFrom(const CImage& other);
    void        CopyRectFrom(const CImage& other, glm::ivec2 posMin, glm::ivec2 posMax);

    void        SetPixelColor(const glm::uvec2& pos, const glm::uvec3& color);
    glm::uvec3  GetPixelColor(const glm::uvec2& pos) const;

    std::size_t CountUsedBytes() const;
    bool        CoordinateWithinImage(const glm::uvec2& pos) const;

    const pixelsPtr& GetRawPixels() const;

private:
    void           AllocatePixels();
    unsigned char* GetPixelPtr(const glm::uvec2& pos) const;

    pixelsPtr  pixels;
    glm::uvec2 imageSize;
};

}

#endif
