#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "coding-base.hpp"
#include "context.hpp"

namespace coding {

class ArithmeticPPM: public Base {
public:
    inline static const std::string TYPE = "arithmetic-ppm";
    using History = std::deque<uint16_t>;

    const uint16_t bits, charset;
    const int order;

    Context* root;

    ArithmeticPPM();
    ~ArithmeticPPM();

    Context* find(const History&);
    void update(const History&, uint16_t);

    size_t encode(DataSrc&, DataDst&) final;
    size_t decode(DataSrc&, DataDst&) final;
};

}
