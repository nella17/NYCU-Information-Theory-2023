#pragma once

#include <unistd.h>
#include <limits>
#include <bitset>
#include <vector>
#include "data.hpp"
#include "datasrc.hpp"

class Arithmetic {
public:
    static constexpr uint64_t BITS = 32;
    static constexpr uint64_t MAX = (1ll << BITS) - 1;

    const std::vector<uint32_t> accum;
    const uint32_t size;
    std::bitset<BITS> L, U;
    int scale3;
    Arithmetic(std::vector<uint32_t>&);

    DataType send(uint32_t);
};
