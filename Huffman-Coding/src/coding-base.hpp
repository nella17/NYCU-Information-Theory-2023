#pragma once

#include <string>

#include "datasrc.hpp"
#include "datadst.hpp"

namespace coding {

class Base {
public:
    virtual void encode(DataSrc&, DataDst&) = 0;
    virtual void decode(DataSrc&, DataDst&) = 0;
};

}
