#ifndef MYPAINT_PLUGIN_MAIN_H
#define MYPAINT_PLUGIN_MAIN_H
#include "plugin_api.h"
#include <tools/tool.h>
#include <image_formats/loader.h>

typedef mp::CTool*(*toolFactoryPtr)(mp::CMyPaint*);
typedef mp::CImageLoader*(*loaderFactoryPtr)();

#define ARRAYCOUNT(array) (sizeof(array)/sizeof((array)[0]))

/**
 * These symbols use "C"-naming and should be the same in all
 * plugins. They store pointers to factories that create custom
 * tools and image loaders
 */
extern "C" PLUGIN_EXPORT toolFactoryPtr   plugin_ToolFactories[];
extern "C" PLUGIN_EXPORT loaderFactoryPtr plugin_LoaderFactories[];
extern "C" PLUGIN_EXPORT int toolFactoryNumElements;
extern "C" PLUGIN_EXPORT int loaderFactoryNumElements;

#endif
