#pragma once

#include <string>

#include "datasource.hpp"

namespace coding {

class Base {
public:
    virtual std::string encode(const DataSource&) = 0;
    virtual std::string decode(const std::string&) = 0;
};

}
