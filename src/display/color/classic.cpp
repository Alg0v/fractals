//
// Created by algov on 9/1/24.
//

#include <cstdint>
#include <utility>

namespace color{
    int classic(const std::pair<uint8_t, double>& data){
        return (std::get<0>(data) * 16 << 16) + 128;
    }
}