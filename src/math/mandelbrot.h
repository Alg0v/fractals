//
// Created by algov on 8/28/24.
//

#ifndef UNTITLED_MANDELBROT_H
#define UNTITLED_MANDELBROT_H
#include "./complex.h"
#include <cstdint>
#include <utility>

namespace math{
    extern std::pair<uint8_t, double> mandelbrot(const Complex& c, const uint8_t& iterations);
}

#endif //UNTITLED_MANDELBROT_H
