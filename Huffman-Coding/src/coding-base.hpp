#pragma once

#include <string>

#include "datasource.hpp"

namespace coding {

class Base {
public:
    virtual std::string encode(DataSource&) = 0;
    virtual std::string decode(DataSource&) = 0;
};

}
