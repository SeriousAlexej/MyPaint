cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

if(WIN32)
    list(APPEND ADDITIONAL_LIBRARIES gdi32 opengl32 imm32)
elseif(UNIX)
    list(APPEND ADDITIONAL_LIBRARIES X11 Xrandr Xinerama Xxf86vm Xcursor dl GL pthread)
    set(CMAKE_INSTALL_RPATH "\$ORIGIN")
endif()

set(GLAD "${GLFW_SOURCE_DIR}/deps/glad/glad.h"
         "${GLFW_SOURCE_DIR}/deps/glad.c")

set(IMGUI "${IMGUI_SOURCE_DIR}/examples/opengl_example/imgui_impl_glfw.h"
          "${IMGUI_SOURCE_DIR}/examples/opengl_example/imgui_impl_glfw.cpp"
          "${IMGUI_SOURCE_DIR}/imgui.h"
          "${IMGUI_SOURCE_DIR}/imgui.cpp"
          "${IMGUI_SOURCE_DIR}/imgui_demo.cpp"
          "${IMGUI_SOURCE_DIR}/imgui_draw.cpp"
          "${IMGUI_SOURCE_DIR}/imconfig.h")

set(TINYFD "${TINYFD_SOURCE_DIR}/tinyfiledialogs.c"
           "${TINYFD_SOURCE_DIR}/tinyfiledialogs.h")

set(SRC_LIST
    "main.cpp"
    "mypaintgui.cpp"
    "mypaintgui.h")

add_executable(MyPaintGUI ${SRC_LIST} ${GLAD} ${IMGUI} ${TINYFD})
target_link_libraries(MyPaintGUI MyPaint glfw ${ADDITIONAL_LIBRARIES})

install(TARGETS MyPaintGUI
        RUNTIME DESTINATION "${CMAKE_SOURCE_DIR}/output")
