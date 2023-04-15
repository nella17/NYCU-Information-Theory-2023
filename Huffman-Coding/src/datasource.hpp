#pragma once

#include <unistd.h>
#include <vector>
#include <string>

#include "options.hpp"
#include "utils.hpp"

class DataSource {
    const bool stream;
    size_t datacur, datasize;
    uint8_t* data;
    DataType remain;
public:

    DataSource();

    bool empty() const;
    void reset();

    bool operator[](size_t) const;
    DataType read(size_t);
    DataType read(size_t, size_t) const;

    uint64_t readint(size_t);
};
