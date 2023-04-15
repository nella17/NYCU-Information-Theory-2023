#include "datasource.hpp"

#include <cassert>
#include <iostream>
#include <sys/mman.h>

#include "options.hpp"

DataSource::DataSource():
    stream(opts.stream),
    datacur(0),
    remain{}
{
    if (!stream) {
        if (lseek(opts.input_fd, 0, SEEK_SET) < 0) {
            perror("lseek");
            exit(EXIT_FAILURE);
        }
        size_t size = (size_t)lseek(opts.input_fd, 0, SEEK_END);
        datasize = size * 8;
        auto pagesize = (size_t)sysconf(_SC_PAGE_SIZE);
        auto mapsize = (size + pagesize - 1) & (~(pagesize-1));
        data = (uint8_t*)mmap(NULL, mapsize, PROT_READ, MAP_FILE | MAP_SHARED, opts.input_fd, 0);
        if ((long)data < 0) perror("mmap");
    }
}

bool DataSource::empty() const {
    if (!stream) {
        return datacur < datasize;
    } else {
        // TODO
        __builtin_unreachable();
    }
}

void DataSource::reset() {
    if (!stream) {
        datacur = 0;
    } else {
        assert(false);
    }
}

bool DataSource::operator[](size_t idx) const {
    if (!stream) {
        return (data[idx / 8] >> (idx % 8)) & 1;
    } else {
        assert(false);
    }
}

DataType DataSource::read(size_t bits) {
    if (!stream) {
        auto r = read(datacur, bits);
        datacur += bits;
        return r;
    return r;
    } else {
        // TODO
        __builtin_unreachable();
    }
}

DataType DataSource::read(size_t pos, size_t bits) const {
    if (!stream) {
        DataType dt(bits, 0);
        for (size_t i = 0, j = pos; i < bits; i++, j++)
            dt[i] = operator[](j);
    return dt;
    } else {
        assert(false);
    }
}

uint64_t DataSource::readint(size_t bits) {
    if (!stream) {
        assert(datacur % 8 == 0 and bits % 8 == 0);
        uint64_t ret = 0;
        for (size_t i = 0; i < bits and datacur < datasize; i += 8, datacur += 8)
            ret = (ret << 8) | data[datacur / 8];
        return ret;
    } else {
        // TODO
        __builtin_unreachable();
    }
}

