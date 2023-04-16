#pragma once

#include "coding-base.hpp"

namespace coding {

class Adaptive: public Base {
public:
    inline static const std::string TYPE = "adaptive";

    Adaptive();
    ~Adaptive();
    size_t encode(DataSrc&, DataDst&) final;
    size_t decode(DataSrc&, DataDst&) final;
};

}
