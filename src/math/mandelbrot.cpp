//
// Created by algov on 8/28/24.
//

#include <cstdint>
#include <utility>
#include "./complex.h"

namespace math{
    std::pair<uint8_t, double> mandelbrot(const Complex& c, const uint8_t& iterations){
        uint8_t in_radius{0}; // number of terms within the list
        Complex z = Complex(0, 0);

        z = pow(z, 2) - c;

        while (z.modulo_squared() < 10 && in_radius < iterations){
            in_radius++;
            z = pow(z, 2) - c;
        }

        return std::make_pair(in_radius, z.modulo_squared());
    }
}