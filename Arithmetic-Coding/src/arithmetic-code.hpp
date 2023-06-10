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

    std::bitset<BITS> L, U, T;
    int scale3;
    Arithmetic();

    void update(uint32_t, uint32_t, uint32_t);
    DataType send(const std::vector<uint32_t>&, uint32_t);
    DataType send(uint32_t, uint32_t, uint32_t);

    uint32_t recv(DataSrc&, const std::vector<uint32_t>&);
    void recv(DataSrc&, uint32_t, uint32_t, uint32_t);
};
