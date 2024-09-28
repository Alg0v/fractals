//
// Created by algov on 9/1/24.
//

#ifndef UNTITLED_JULIA_H
#define UNTITLED_JULIA_H

#include <cstdint>
#include "./complex.h"
#include <utility>

namespace math{
    extern Complex julia_c;

    extern std::pair<uint8_t, double> julia(const Complex& c, const uint8_t& iterations);
}

#endif //UNTITLED_JULIA_H
