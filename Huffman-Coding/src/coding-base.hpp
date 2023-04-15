#pragma once

#include <string>

#include "datasrc.hpp"
#include "datadst.hpp"

namespace coding {

class Base {
public:
    virtual ~Base() = 0;
    virtual void encode(DataSrc&, DataDst&) = 0;
    virtual void decode(DataSrc&, DataDst&) = 0;
};

}
