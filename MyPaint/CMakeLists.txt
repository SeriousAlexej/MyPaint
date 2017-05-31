cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

set(SRC_LIST
    "event_manager/eventmanager.h"
    "event_manager/eventmanager.cpp"
    "event_manager/eventsubscriber.h"
    "event_manager/events.h"
    "mypaint.cpp"
    "mypaint.h"
    "mypaint_api.h"
    "image/image.cpp"
    "image/image.h"
    "tools/tool.h"
    "image_formats/loader.h"
    "dirtools/dirtools.h"
    "dirtools/dirtools.cpp"
    "plugin_loaders/pluginLoader.h"
    "plugin_loaders/pluginLoader.cpp")

add_library(MyPaint SHARED ${SRC_LIST})

if (WIN32)
    if (MINGW)
        # Remove the lib prefix on the DLL (but not the import library
        set_target_properties(MyPaint PROPERTIES PREFIX "")

        # Add a suffix to the import library to avoid naming conflicts
        set_target_properties(MyPaint PROPERTIES IMPORT_SUFFIX "dll.a")
    else()
        # Add a suffix to the import library to avoid naming conflicts
        set_target_properties(MyPaint PROPERTIES IMPORT_SUFFIX "dll.lib")
    endif()
endif()

target_compile_definitions(MyPaint PUBLIC "-DMYPAINT_BUILDING_DLL")

if (WIN32)
    install(TARGETS MyPaint
            RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/output")
else()
    install(TARGETS MyPaint
            LIBRARY DESTINATION "${CMAKE_SOURCE_DIR}/output")
endif()
		
