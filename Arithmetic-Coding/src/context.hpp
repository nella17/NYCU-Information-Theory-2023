#pragma once

#include <iostream>
#include <bitset>
#include <map>
#include <numeric>
#include <vector>
#include <utility>
#include <deque>

class Context {
public:
    static constexpr uint16_t MAX_CHARSET = 257;
    using Range = std::tuple<uint16_t, uint16_t, uint16_t>;
    using Bits = std::bitset<MAX_CHARSET>;

    Context* const parent;
    const int order;
    const uint16_t charset;
    size_t size;
    Bits used;
    std::vector<uint32_t> count;
    std::vector<Context*> next;

    Context(Context*, int, uint16_t);
    void inc(uint16_t);
    bool has(uint16_t);
    Range range(uint16_t, Bits = 0);
    Context* get(uint16_t);
};

std::ostream& operator<<(std::ostream&, const Context&);
