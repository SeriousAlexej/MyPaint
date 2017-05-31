#include "dirtools.h"

#if defined(_WIN32)
#include "../../external_libs/dirent/include/dirent.h"
#else
#include <dirent.h>
#endif

namespace mp
{

namespace dir
{

static std::string GetLibExtension()
{
#if defined(_WIN32)
    return ".dll";
#else
    return ".so";
#endif
}

char GetDirDelimiter()
{
#if defined(_WIN32)
    return '\\';
#else
    return '/';
#endif
}

std::vector<std::string> GetLibrariesInDir(const std::string &dirname)
{
    std::vector<std::string> files;
    const std::string libExt = GetLibExtension();
    std::size_t extLen = libExt.length();

    DIR *dir;
    struct dirent *ent;

    dir = opendir(dirname.c_str());
    if (dir != nullptr)
    {
        while ((ent = readdir(dir)) != nullptr)
        {
            if(ent->d_type == DT_REG)
            {
                std::string fnm = ent->d_name;
                if(fnm.length() > extLen && fnm.substr(fnm.length() - extLen) == libExt)
                {
                    files.push_back(fnm);
                }
            }
        }

        closedir(dir);
    }

    return files;
}

}

}
