#pragma once

#include <unistd.h>
#include <vector>
#include <string>

#include "data.hpp"
#include "utils.hpp"

class DataDst {
    const int fd;
    size_t size;
    std::basic_string<uint8_t> buf;
public:
    DataDst(int);
    ~DataDst();

    void write(bool = false);
    void write(const DataType&);
    void write(Data&&);
    void writeint(size_t, uint64_t, bool = true);
};
