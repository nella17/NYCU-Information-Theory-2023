#pragma once

#include <unistd.h>
#include <vector>
#include <string>

#include "options.hpp"
#include "utils.hpp"

class DataDst {
public:
    DataDst();

    void write(const DataType&);
    void writeint(uint64_t, size_t);
};
