#pragma once

#include <string>

#include "coding-base.hpp"

namespace coding {

class ArithmeticPPM: public Base {
public:
    inline static const std::string TYPE = "arithmetic-ppm";

    ArithmeticPPM();
    ~ArithmeticPPM();
    size_t encode(DataSrc&, DataDst&) final;
    size_t decode(DataSrc&, DataDst&) final;
};

}
