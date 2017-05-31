cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

set(SRC_LIST
    "plugin_api.h"
    "main.cpp"
    "main.h"
    "tools/toolLine.h"
    "tools/toolLine.cpp"
    "tools/toolRectangle.h"
    "tools/toolRectangle.cpp"
    "tools/toolEllipse.h"
    "tools/toolEllipse.cpp"
    "image_formats/tgaLoader.cpp"
    "image_formats/tgaLoader.h")

add_library(MyPaintDefaultTools SHARED ${SRC_LIST})
target_link_libraries(MyPaintDefaultTools MyPaint)

if (WIN32)
    if (MINGW)
        # Remove the lib prefix on the DLL (but not the import library
        set_target_properties(MyPaintDefaultTools PROPERTIES PREFIX "")

        # Add a suffix to the import library to avoid naming conflicts
        set_target_properties(MyPaintDefaultTools PROPERTIES IMPORT_SUFFIX "dll.a")
    else()
        # Add a suffix to the import library to avoid naming conflicts
        set_target_properties(MyPaintDefaultTools PROPERTIES IMPORT_SUFFIX "dll.lib")
    endif()
endif()

if (WIN32)
    install(TARGETS MyPaintDefaultTools
            RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/output/plugins")
else()
    install(TARGETS MyPaintDefaultTools
            LIBRARY DESTINATION "${CMAKE_SOURCE_DIR}/output/plugins")
endif()
