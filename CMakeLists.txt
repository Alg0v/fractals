cmake_minimum_required(VERSION 3.28)
project(fractals)


set(CMAKE_CXX_STANDARD 17)

include_directories(lib_imgui)

add_executable(fractals src/main.cpp
        src/display/GUI.cpp
        src/math/mandelbrot.cpp
        src/math/complex.cpp
        src/math/utils.h
        src/math/utils.cpp
        src/math/julia.h
        src/math/julia.cpp
        src/display/color/classic.h
        src/display/color/classic.cpp
        src/display/color/algov.h
        src/display/color/algov.cpp
        src/display/color/algov_alt.h
        src/display/color/algov_alt.cpp
        src/display/color/algov_stat.h
        src/display/color/algov_stat.cpp
        lib_imgui/backends/imgui_impl_sdl2.h
        lib_imgui/backends/imgui_impl_sdlrenderer2.h
        lib_imgui/backends/imgui_impl_sdl2.cpp
        lib_imgui/backends/imgui_impl_sdlrenderer2.cpp
)

target_link_libraries(fractals imgui)
target_link_libraries(fractals SDL2)
