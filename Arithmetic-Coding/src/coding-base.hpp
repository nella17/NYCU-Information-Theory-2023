#pragma once

#include <string>

#include "datasrc.hpp"
#include "datadst.hpp"

namespace coding {

class Base {
public:
    virtual ~Base() = 0;
    virtual size_t encode(DataSrc&, DataDst&) = 0;
    virtual size_t decode(DataSrc&, DataDst&) = 0;
};

}
