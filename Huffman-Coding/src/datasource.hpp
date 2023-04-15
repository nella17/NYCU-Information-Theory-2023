#pragma once

#include <unistd.h>
#include <vector>
#include <string>

#include "options.hpp"
#include "utils.hpp"
#include "data.hpp"

class DataSource {
    const bool stream;
    DataType remain;
    Data data;
public:

    DataSource();

    bool empty() const;
    void reset();
    void clear();

    bool operator[](size_t) const;
    DataType read(size_t);
    DataType read(size_t, size_t) const;

    uint64_t readint(size_t);
};
