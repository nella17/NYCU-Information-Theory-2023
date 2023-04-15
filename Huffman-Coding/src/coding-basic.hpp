#pragma once

#include <string>

#include "coding-base.hpp"

namespace coding {

class Basic: public Base {
public:
    inline static const std::string TYPE = "basic";

    Basic();
    std::string encode(DataSource&) final;
    std::string decode(DataSource&) final;
};

}
