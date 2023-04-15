#pragma once

#include <string>

#include "coding-base.hpp"

namespace coding {

class Basic: public Base {
public:
    inline static const std::string TYPE = "basic";

    Basic();
    ~Basic();
    size_t encode(DataSrc&, DataDst&) final;
    size_t decode(DataSrc&, DataDst&) final;
};

}
