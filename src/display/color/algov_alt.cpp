//
// Created by algov on 10/31/24.
//

#include <cstdint>
#include <utility>

namespace color{
    int algov_alt(const std::pair<uint8_t, double>& data){
        return (uint8_t)(std::get<1>(data)) << 16 | std::get<0>(data)*8 | std::get<0>(data)*16;
    }
}