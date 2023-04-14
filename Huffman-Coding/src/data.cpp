#include "data.hpp"

#include <cassert>
#include <iostream>

Data::Data(std::string s): cur(0), size(s.size() * 8), data(s.begin(), s.end()) {}

void Data::reset() {
    cur = 0;
}

void Data::clear() {
    cur = 0;
    size = 0;
    data.clear();
}

bool Data::operator[](size_t idx) const {
    return (data[idx / 8] >> (idx % 8)) & 1;
}

DataType Data::read(size_t bits) {
    auto r = read(cur, bits);
    cur += bits;
    return r;
}

DataType Data::read(size_t pos, size_t bits) const {
    DataType dt(bits, 0);
    for (size_t i = 0, j = pos; i < bits; i++, j++)
        dt[i] = operator[](j);
    return dt;
}

uint64_t Data::readint(size_t bits) {
    assert(cur % 8 == 0 and bits % 8 == 0);
    uint64_t ret = 0;
    for (size_t i = 0; i < bits; i += 8, cur += 8)
        ret = (ret << 8) | data[cur/8];
    return ret;
}

void Data::push(const bool b) {
    if (size % 8 == 0)
        data.append(0);
    data[size / 8] |= b << (size % 8);
    size += 1;
}

Data& Data::operator+=(const Data& dt) {
    for (size_t i = 0; i < dt.size; i++)
        push(dt[i]);
    return *this;
}

Data& Data::operator+=(const DataType& dt) {
    for (auto v: dt)
        push(v);
    return *this;
}
