#include "datadst.hpp"

#include <cassert>
#include <iostream>
#include <sys/mman.h>

DataDst::DataDst(int _fd): fd(_fd), size(0), buf() {}
DataDst::~DataDst() {
    write(true);
}

void DataDst::write(bool all) {
    if (buf.empty()) return;
    assert(buf.size() - size / 8 <= 1);
    if (::write(fd, buf.c_str(), (size + (all ? 7 : 0)) / 8) < 0) {
        perror("write");
        exit(EXIT_FAILURE);
    }
    auto l = buf.back();
    buf.clear();
    if (!all and size % 8)
        buf.push_back(l);
    size %= 8;
}

void DataDst::write(const DataType& data) {
    size_t i = 0, sz = data.size();
    if (size % 8) {
        for (; i < sz and size % 8; i++, size++)
            buf.back() |= uint8_t(data[i] << (7 - size % 8));
    }
    for (; i+8 <= sz; i += 8, size += 8) {
        uint8_t t = 0;
        for (size_t j = 0; j < 8; j++)
            t = uint8_t(t << 1) | (uint8_t)data[i+j];
        buf.push_back(t);
    }
    if (i < sz) {
        buf.push_back(0);
        for (; i < sz; i++, size++)
            buf.back() |= uint8_t(data[i] << (7 - size % 8));
    }
    write();
}

void DataDst::write(Data& data) {
    data.reset();
    if (size % 8) {
        auto x = 8 - size % 8;
        writeint(x, data.readint(x), false);
    }
    for (; data.remain() >= 8; size += 8)
        buf.push_back((uint8_t)data.readint(8));
    auto r = data.remain();
    if (r)
        writeint(r, data.readint(r), false);
    write();
}

void DataDst::writeint(size_t bits, uint64_t data, bool w) {
    std::bitset<64> bs(data);
    size_t i = 0;
    for (; i < bits and size % 8; i++, size++)
        buf.back() |= uint8_t(bs[bits-1-i] << (7 - size % 8));
    for (; i + 8 <= bits; i += 8, size += 8)
        buf.push_back((data >> (bits-8-i)) & 0xFF);
    if (i < bits) {
        buf.push_back(0);
        for (; i < bits; i++, size++)
            buf.back() |= uint8_t(bs[bits-1-i] << (7 - size % 8));
    }
    if (w) write();
}

