#ifndef PLUGIN_API_H
#define PLUGIN_API_H

#if defined(_WIN32)
 #define PLUGIN_EXPORT __declspec(dllexport)
#elif defined(__GNUC__)
 #define PLUGIN_EXPORT __attribute__((visibility("default")))
#else
 #define PLUGIN_EXPORT
#endif

#endif
