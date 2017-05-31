cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11)

set(GLFW_SOURCE_DIR   "${CMAKE_SOURCE_DIR}/external_libs/glfw-3.2.1")
set(IMGUI_SOURCE_DIR  "${CMAKE_SOURCE_DIR}/external_libs/imgui-1.49")
set(TINYFD_SOURCE_DIR "${CMAKE_SOURCE_DIR}/external_libs/tiny_file_dialogs-2.7.3")
set(GLM_SOURCE_DIR    "${CMAKE_SOURCE_DIR}/external_libs/glm")

include_directories("${GLFW_SOURCE_DIR}/deps"
                    "${GLFW_SOURCE_DIR}/include"
                    "${IMGUI_SOURCE_DIR}"
					"${IMGUI_SOURCE_DIR}/examples/opengl_example"
                    "${TINYFD_SOURCE_DIR}"
                    "${GLM_SOURCE_DIR}"
                    "${CMAKE_SOURCE_DIR}/MyPaint")

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "Build the GLFW example programs")
set(GLFW_BUILD_TESTS OFF CACHE BOOL  "Build the GLFW test programs")
set(GLFW_BUILD_DOCS OFF CACHE BOOL  "Build the GLFW documentation")
set(GLFW_INSTALL OFF CACHE BOOL  "Generate installation target")
add_subdirectory("${GLFW_SOURCE_DIR}" "${GLFW_SOURCE_DIR}/build")

add_subdirectory(MyPaint)

add_subdirectory(MyPaintGUI)

add_subdirectory(MyPaintDefaultTools)

add_subdirectory(MyPaintPluginPen)
