#ifndef PLUGIN_LOADER_H
#define PLUGIN_LOADER_H
#include <string>
#if defined(_WIN32)
#include <windows.h>
#endif

namespace mp
{

/**
 * CPluginLoader class wraps the loading
 * of shared libraries and finding symbols in them.
 * Used to load plugins in runtime
 */
class CPluginLoader
{
public:
    explicit CPluginLoader(const std::string& libPath);
    ~CPluginLoader();

    void* FindSymbol(const std::string& symbol);

private:
#if defined(_WIN32)
    HMODULE     library;
#else
    void*       library;
#endif
};

}

#endif
