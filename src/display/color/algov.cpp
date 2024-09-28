//
// Created by algov on 9/1/24.
//

#include <utility>
#include <cstdint>
#include <cmath>

namespace color{
    int algov(const std::pair<uint8_t, double>& data){
        if(std::get<1>(data) < 10){
            return (int)(128 + 1300*(sqrt(std::get<1>(data)) - 0.5)) << 16 | 255;
        }
        return 0;
    }
}