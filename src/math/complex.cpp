//
// Created by algov on 8/28/24.
//

#include <cstdint>
#include "./complex.h"

namespace math {
    Complex::Complex(const double &a, const double &b) {
        x = a;
        y = b;
    }

    double Complex::modulo_squared() const {
        return x * x + y * y;
    }

    Complex Complex::operator+(const Complex &z) const {
        return {x + z.x, y + z.y};
    }

    Complex Complex::operator-(const Complex &z) const {
        return {x - z.x, y - z.y};
    }

    Complex Complex::operator*(const Complex &z) const {
        return {x * z.x - y * z.y, x * z.y + y * z.x};
    }
}

math::Complex pow(math::Complex z, uint8_t n) {
    for (int i = 0; i < n-1; i++) z = z * z;
    return z;
}