#pragma once

#include <string>

#include "data.hpp"

namespace coding {

class Base {
public:
    virtual std::string encode(const Data&) = 0;
    virtual std::string decode(const std::string&) = 0;
};

}
