#pragma once

#include <string>

#include "coding-base.hpp"

namespace coding {

class ArithmeticFPM: public Base {
public:
    inline static const std::string TYPE = "arithmetic-fpm";

    ArithmeticFPM();
    ~ArithmeticFPM();
    size_t encode(DataSrc&, DataDst&) final;
    size_t decode(DataSrc&, DataDst&) final;
};

}
