#pragma once

#include <unistd.h>
#include <vector>
#include <bitset>
#include <string>

#include "utils.hpp"

class Data {
    friend class DataSrc;

    size_t datacur, datasize;
    std::basic_string<uint8_t> data;
public:
    Data(std::string = "");

    bool eof() const;
    size_t remain() const;
    size_t size() const;
    void reset();
    void clear();

    bool operator[](size_t) const;
    DataType read(size_t);
    DataType read(size_t, size_t) const;

    uint64_t readint(size_t);

    void write(const DataType&);
    void writeint(size_t, uint64_t);
};
