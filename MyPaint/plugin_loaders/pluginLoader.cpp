#include <stdexcept>
#include "pluginLoader.h"
#if !defined(_WIN32)
#include <dlfcn.h>
#endif

namespace mp
{

CPluginLoader::CPluginLoader(const std::string &libPath)
{
#if defined(_WIN32)
    library = LoadLibraryA(libPath.c_str());
    if(!library)
    {
        throw std::logic_error(std::string("Error ") + std::to_string(GetLastError()));
    }
#else
    library = dlopen(libPath.c_str(), RTLD_LAZY | RTLD_GLOBAL);
    char* error = dlerror();
    if(error)
    {
        throw std::logic_error(error);
    }
#endif
}

CPluginLoader::~CPluginLoader()
{
    if(library)
    {
#if defined(_WIN32)
        FreeLibrary(library);
#else
        dlclose(library);
#endif
    }
}

void* CPluginLoader::FindSymbol(const std::string &symbol)
{
#if defined(_WIN32)
    void* retval = (void*)GetProcAddress(library, symbol.c_str());
    if(!retval)
    {
        throw std::logic_error(std::string("Error ") + std::to_string(GetLastError()));
    }
#else
    void* retval = dlsym(library, symbol.c_str());
    char* error = dlerror();
    if(error)
    {
        throw std::logic_error(error);
    }
#endif
    return retval;
}

}
