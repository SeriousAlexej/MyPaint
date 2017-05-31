cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

set(SRC_LIST
    "plugin_api.h"
    "main.cpp"
    "main.h"
    "tools/toolPen.h"
    "tools/toolPen.cpp")

add_library(MyPaintPluginPen SHARED ${SRC_LIST})
target_link_libraries(MyPaintPluginPen MyPaint)

if (WIN32)
    if (MINGW)
        # Remove the lib prefix on the DLL (but not the import library
        set_target_properties(MyPaintPluginPen PROPERTIES PREFIX "")

        # Add a suffix to the import library to avoid naming conflicts
        set_target_properties(MyPaintPluginPen PROPERTIES IMPORT_SUFFIX "dll.a")
    else()
        # Add a suffix to the import library to avoid naming conflicts
        set_target_properties(MyPaintPluginPen PROPERTIES IMPORT_SUFFIX "dll.lib")
    endif()
endif()

if (WIN32)
    install(TARGETS MyPaintPluginPen
            RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/output/plugins")
else()
    install(TARGETS MyPaintPluginPen
            LIBRARY DESTINATION "${CMAKE_SOURCE_DIR}/output/plugins")
endif()
