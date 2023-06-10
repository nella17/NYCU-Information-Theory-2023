#pragma once

#include <unistd.h>
#include <limits>
#include <bitset>
#include "data.hpp"

class Arithmetic {
public:
    static constexpr uint64_t BITS = 31;
    static constexpr uint64_t MAX = (1ll << BITS) - 1;
    static constexpr uint64_t MAX_HALF = MAX / 2;
    static constexpr uint64_t MAX14 = MAX * 1 / 4;
    static constexpr uint64_t MAX34 = MAX * 3 / 4;

    Data& data;
    uint64_t L, U;
    int scale3;
    Arithmetic(Data&);

    void send(uint64_t, uint64_t, uint64_t);
    void scaling();
};
