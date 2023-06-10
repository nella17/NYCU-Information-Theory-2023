#pragma once

#include <string>

#include "coding-base.hpp"

namespace coding {

class ArithmeticFPM: public Base {
public:
    inline static const std::string TYPE = "arithmetic-fpm";

    const size_t bits;
    const size_t charset;
    std::vector<uint32_t> prob, accum, s2i, i2s;

    void build();

    ArithmeticFPM();
    ~ArithmeticFPM();
    size_t encode(DataSrc&, DataDst&) final;
    size_t decode(DataSrc&, DataDst&) final;
};

}
