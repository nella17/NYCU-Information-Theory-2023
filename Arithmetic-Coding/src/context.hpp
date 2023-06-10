#pragma once

#include <iostream>
#include <bitset>
#include <map>
#include <numeric>
#include <vector>
#include <utility>
#include <deque>
#include "arithmetic-code.hpp"

class Context {
public:
    static constexpr uint32_t MAX_CHARSET = 257;
    using Range = std::tuple<uint32_t, uint32_t, uint32_t>;
    using Bits = std::bitset<MAX_CHARSET>;

    Context* const parent;
    const int order;
    const uint32_t charset;
    size_t size;
    Bits used;
    std::vector<uint32_t> count;
    std::vector<Context*> next;

    Context(Context*, int, uint32_t);
    void inc(uint32_t);
    bool has(uint32_t);
    Range range(uint32_t, Bits);
    Context* get(uint32_t);
    Arithmetic::Accum getacc(Bits);
    uint32_t i2s(uint32_t, Bits);
};

std::ostream& operator<<(std::ostream&, const Context&);
