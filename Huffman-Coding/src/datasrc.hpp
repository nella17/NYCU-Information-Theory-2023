#pragma once

#include <unistd.h>
#include <vector>
#include <string>

#include "data.hpp"
#include "utils.hpp"

class DataSrc {
    const bool stream;
    const int fd;
    size_t split;
    size_t datacur, filesize, start, end;
    uint8_t* data;
    DataType buf;
public:

    DataSrc(bool, int);

    bool eof() const;
    size_t remain() const;
    size_t size() const;
    size_t total() const;
    void reset();
    void back(size_t);

    void resplit(size_t);
    bool nextsplit();

    bool operator[](size_t) const;
    DataType read(size_t);
    DataType read(size_t, size_t) const;

    Data readdata(const size_t);

    uint64_t readint(size_t);
};
