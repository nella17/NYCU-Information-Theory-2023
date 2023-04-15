#include "datasrc.hpp"

#include <cassert>
#include <iostream>
#include <sys/mman.h>

DataSrc::DataSrc(bool _s, int _fd):
    stream(_s),
    fd(_fd),
    datacur(0),
    remain{}
{
    if (!stream) {
        if (lseek(fd, 0, SEEK_SET) < 0) {
            perror("lseek");
            exit(EXIT_FAILURE);
        }
        size_t size = (size_t)lseek(fd, 0, SEEK_END);
        datasize = size * 8;
        auto pagesize = (size_t)sysconf(_SC_PAGE_SIZE);
        auto mapsize = (size + pagesize - 1) & (~(pagesize-1));
        data = (uint8_t*)mmap(NULL, mapsize, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
        if ((long)data < 0) perror("mmap");
    }
}

bool DataSrc::eof() const {
    if (!stream) {
        return datacur < datasize;
    } else {
        // TODO
        assert(!(bool)"TODO");
    }
}

size_t DataSrc::size() const {
    if (!stream) {
        return datasize;
    } else {
        assert(false);
    }
}

void DataSrc::reset() {
    if (!stream) {
        datacur = 0;
    } else {
        assert(false);
    }
}

bool DataSrc::operator[](size_t idx) const {
    if (!stream) {
        return (data[idx / 8] >> (7 - idx % 8)) & 1;
    } else {
        assert(false);
    }
}

DataType DataSrc::read(size_t bits) {
    if (!stream) {
        auto r = read(datacur, bits);
        datacur += bits;
        return r;
    } else {
        // TODO
        assert(!(bool)"TODO");
    }
}

DataType DataSrc::read(size_t pos, size_t bits) const {
    if (!stream) {
        DataType dt(bits, 0);
        for (size_t i = 0, j = pos; i < bits; i++, j++)
            dt[i] = operator[](j);
        return dt;
    } else {
        assert(false);
    }
}

uint64_t DataSrc::readint(size_t bits) {
    if (!stream) {
        uint64_t ret = 0;
        size_t i = 0;
        for (; i < bits and datacur % 8; i++, datacur++)
            ret = (ret << 1) | operator[](datacur);
        for (; i + 8 <= bits and datacur + 8 <= datasize; i += 8, datacur += 8)
            ret = (ret << 8) | data[datacur / 8];
        for (; i < bits; i++, datacur++)
            ret = (ret << 1) | operator[](datacur);
        return ret;
    } else {
        // TODO
        assert(!(bool)"TODO");
    }
}

