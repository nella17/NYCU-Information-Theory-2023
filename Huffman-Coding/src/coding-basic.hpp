#pragma once

#include <string>

#include "coding-base.hpp"

namespace coding {

class Basic: public Base {
public:
    inline static const std::string TYPE = "basic";

    Basic();
    void encode(DataSrc&, DataDst&) final;
    void decode(DataSrc&, DataDst&) final;
};

}
