#pragma once

#include <unistd.h>
#include <vector>
#include <bitset>
#include <string>

using DataType = std::basic_string<bool>;

class Data {
public:
    using Value = std::bitset<8>;

    size_t cur, size;
    std::basic_string<uint8_t> data;

    Data(std::string = "");

    void reset();
    void clear();

    bool operator[](size_t) const;
    DataType read(size_t);
    DataType read(size_t, size_t) const;

    uint64_t readint(size_t);

    void push(const bool);
    Data& operator+=(const Data&);
    Data& operator+=(const DataType&);
};
