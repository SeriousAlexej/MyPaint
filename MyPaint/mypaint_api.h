#ifndef MY_PAINT_API_H
#define MY_PAINT_API_H

#if defined(_WIN32) && defined(MYPAINT_BUILDING_DLL)
 #define MYPAINT_API __declspec(dllexport)
#elif defined(_WIN32)
 #define MYPAINT_API __declspec(dllimport)
#elif defined(__GNUC__) && defined(MYPAINT_BUILDING_DLL)
 #define MYPAINT_API __attribute__((visibility("default")))
#else
 #define MYPAINT_API
#endif

#endif
