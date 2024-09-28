cmake_minimum_required(VERSION 3.28)
project(fractals)

set(CMAKE_CXX_STANDARD 17)

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
        src/display/color/algov.cpp)

target_link_libraries(fractals SDL2)
