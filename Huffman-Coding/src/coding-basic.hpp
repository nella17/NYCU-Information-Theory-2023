#pragma once

#include <string>

#include "coding-base.hpp"

namespace coding {

class Basic: public Base {
    size_t bits;
public:
    inline static const std::string TYPE = "basic";

    Basic(const Options&);
    std::string encode(DataSource&) final;
    std::string decode(DataSource&) final;
};

}
