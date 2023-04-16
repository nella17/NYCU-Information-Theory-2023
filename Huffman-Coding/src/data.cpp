#include "data.hpp"

#include <cassert>
#include <iostream>

Data::Data(std::string s): datacur(0), datasize(s.size() * 8), data(s.begin(), s.end()) {}

bool Data::eof() const {
    return datacur >= datasize;
}

size_t Data::remain() const {
    return datasize - datacur;
}

size_t Data::size() const {
    return datasize;
}

void Data::resize(size_t size) {
    datasize = size;
    data.resize((size + 7) / 8);
}

void Data::reset() {
    datacur = 0;
}

bool Data::operator[](size_t idx) const {
    return (data[idx / 8] >> (7 - idx % 8)) & 1;
}

DataType Data::read(size_t bits) {
    auto r = read(datacur, bits);
    datacur += bits;
    return r;
}

DataType Data::read(size_t pos, size_t bits) const {
    DataType dt(bits, 0);
    for (size_t i = 0, j = pos; i < bits; i++, j++)
        dt[i] = operator[](j);
    return dt;
}

uint64_t Data::readint(size_t bits) {
    uint64_t ret = 0;
    size_t i = 0;
    for (; i < bits and datacur % 8; i++, datacur++)
        ret = (ret << 1) | operator[](datacur);
    for (; i + 8 <= bits and datacur + 8 <= datasize; i += 8, datacur += 8)
        ret = (ret << 8) | data[datacur / 8];
    for (; i < bits; i++, datacur++)
        ret = (ret << 1) | operator[](datacur);
    return ret;
}

void Data::write(const DataType& value) {
    size_t i = 0, sz = value.size();
    if (datasize % 8) {
        for (; i < sz and datasize % 8; i++, datasize++)
            data.back() |= uint8_t(value[i] << (7 - datasize % 8));
    }
    for (; i+8 <= sz; i += 8, datasize += 8) {
        uint8_t t = 0;
        for (size_t j = 0; j < 8; j++)
            t = uint8_t(t << 1) | (uint8_t)value[i+j];
        data.push_back(t);
    }
    if (i < sz) {
        data.push_back(0);
        for (; i < sz; i++, datasize++)
            data.back() |= uint8_t(value[i] << (7 - datasize % 8));
    }
}

void Data::writeint(size_t bits, uint64_t value) {
    std::bitset<64> bs(value);
    size_t i = 0;
    for (; i < bits and datasize % 8; i++, datasize++)
        data.back() |= bs[bits-1-i] << (7 - datasize % 8);
    for (; i + 8 <= bits; i += 8, datasize += 8)
        data.push_back((value >> (bits-8-i)) & 0xFF);
    if (i < bits) {
        data.push_back(0);
        for (; i < bits; i++, datasize++)
            data.back() |= bs[bits-1-i] << (7 - datasize % 8);
    }
}
