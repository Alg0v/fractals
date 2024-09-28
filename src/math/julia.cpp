//
// Created by algov on 9/1/24.
//


#include <cstdint>
#include <utility>
#include "./complex.h"

namespace math{
    Complex julia_c = Complex(0, .75);

    std::pair<uint8_t, double> julia(const Complex& c, const uint8_t& iterations){
        Complex z = c;
        uint8_t in_count{0};

        while(z.modulo_squared() < 10 && in_count < iterations){
            z = pow(z, 2) - julia_c;
            in_count++;
        }

        return std::make_pair(in_count, z.modulo_squared());
    }
}