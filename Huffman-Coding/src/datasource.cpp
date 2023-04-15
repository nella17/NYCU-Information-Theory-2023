#include "datasource.hpp"

#include <cassert>
#include <iostream>

DataSource::DataSource(const Options& opts):
    stream(opts.stream),
    remain{},
    data(!opts.stream ? readfile(opts.input_fd) : "")
    {}

bool DataSource::empty() const {
    if (!stream) return data.cur < data.size;
    // TODO
    __builtin_unreachable();
}

void DataSource::reset() {
    if (!stream) data.reset();
    assert(false);
}

void DataSource::clear() {
    if (!stream) data.reset();
    assert(false);
}

bool DataSource::operator[](size_t idx) const {
    if (!stream) return data[idx];
    assert(false);
}

DataType DataSource::read(size_t bits) {
    if (!stream) return data.read(bits);
    // TODO
    __builtin_unreachable();
}

DataType DataSource::read(size_t pos, size_t bits) const {
    if (!stream) return data.read(pos, bits);
    assert(false);
}

uint64_t DataSource::readint(size_t bits) {
    if (!stream) return data.readint(bits);
    // TODO
    __builtin_unreachable();
}

