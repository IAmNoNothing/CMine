cmake_minimum_required(VERSION 4.1)

project(CMine)

include_directories(
    "include"
    "libs/glew-2.1.0/include"
    "libs/glfw-3.4/include"
    "libs/stb"
    "libs/cglm/include"
)

add_executable(
    CMine
    src/main.c
    src/log.c
    src/app.c
    src/window.c
    src/texture.c
    src/shader.c
    src/buffers.c
    src/camera.c
)

target_link_libraries(
    CMine
    "${CMAKE_SOURCE_DIR}/libs/glew-2.1.0/lib/Release/x64/glew32.lib"
    "${CMAKE_SOURCE_DIR}/libs/glfw-3.4/lib-vc2022/glfw3.lib"
    opengl32.lib
    "${CMAKE_SOURCE_DIR}/libs/cglm/bin/Debug/cglm.lib"
)