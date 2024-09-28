//
// Created by algov on 8/28/24.
//

#ifndef UNTITLED_COMPLEX_H
#define UNTITLED_COMPLEX_H

#include <cstdint>

namespace math {
    class Complex{
    public:
        double x, y;
        Complex(const double& a=0, const double& b=0);
        double modulo_squared() const;
        Complex operator+(const Complex& z) const;
        Complex operator-(const Complex& z) const;
        Complex operator*(const Complex& z) const;
    };
}

extern math::Complex pow(math::Complex z, uint8_t n);

#endif //UNTITLED_COMPLEX_H
