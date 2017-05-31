#ifndef IMAGE_LOADER_H
#define IMAGE_LOADER_H
#include <string>
#include "../mypaint_api.h"

namespace mp
{

class CImage;

/**
 * CImageLoader class is an interface for
 * plugins that enable support for various image
 * file formats IO
 */
class MYPAINT_API CImageLoader
{
public:
    CImageLoader() = default;
    virtual ~CImageLoader() = default;

    const std::string& GetFormat() const { return format; }

    virtual void       Write(const std::string& path, const CImage* img) = 0;
    virtual CImage*    Read(const std::string& path) = 0;

protected:
    std::string format = "<unnamed_loader>";
};

}

#endif
