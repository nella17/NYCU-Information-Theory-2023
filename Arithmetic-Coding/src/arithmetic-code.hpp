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
    using Accum = std::vector<uint32_t>;

    std::bitset<BITS> L, U, T;
    size_t scale3;
    Arithmetic();

    DataType end();

    void update(uint32_t, uint32_t, uint32_t);
    DataType send(const Accum&, uint32_t);
    DataType send(uint32_t, uint32_t, uint32_t);

    uint32_t recv(DataSrc&, const Accum&);
    void recv(DataSrc&, uint32_t, uint32_t, uint32_t);
};
