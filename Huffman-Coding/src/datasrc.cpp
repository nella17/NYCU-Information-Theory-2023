#include "datasrc.hpp"

#include <cassert>
#include <iostream>
#include <sys/mman.h>

DataSrc::DataSrc(bool _s, int _fd):
    stream(_s),
    fd(_fd),
    split(INF_SIZET),
    datacur(0),
    start(0),
    buf{}
{
    if (!stream) {
        if (lseek(fd, 0, SEEK_SET) < 0) {
            perror("lseek");
            exit(EXIT_FAILURE);
        }
        size_t size = (size_t)lseek(fd, 0, SEEK_END);
        end = filesize = size;
        auto pagesize = (size_t)sysconf(_SC_PAGE_SIZE);
        auto mapsize = (size + pagesize - 1) & (~(pagesize-1));
        data = (uint8_t*)mmap(NULL, mapsize, PROT_READ, MAP_FILE | MAP_SHARED, fd, 0);
        if ((long)data < 0) {
            perror("mmap");
            exit(EXIT_FAILURE);
        }
    }
}

bool DataSrc::eof() const {
    if (!stream) {
        return datacur / 8 >= end;
    } else {
        // TODO
        assert(!(bool)"TODO");
    }
}

size_t DataSrc::remain() const {
    if (!stream) {
        return end * int64_t(8) - datacur;
    } else {
        assert(false);
    }
}

size_t DataSrc::size() const {
    if (!stream) {
        return (end - start) * 8;
    } else {
        assert(false);
    }
}

size_t DataSrc::total() const {
    return filesize;
}

void DataSrc::reset() {
    if (!stream) {
        datacur = start * 8;
    } else {
        assert(false);
    }
}

void DataSrc::back(size_t k) {
    if (!stream) {
        datacur = std::max(start, datacur - k);
    } else {
        assert(false);
    }
}

void DataSrc::resplit(size_t _sp) {
    split = _sp;
    datacur = 0;
    start = 0;
    end = std::min(filesize, split);
}

bool DataSrc::nextsplit() {
    if (split == INF_SIZET)
        return false;
    if (!stream) {
        if (end == filesize)
            return false;
        start += split;
        datacur = start * 8;
        end = std::min(start + split, filesize);
        return true;
    } else {
        // TODO
        assert(!(bool)"TODO");
    }
}

bool DataSrc::operator[](size_t idx) const {
    if (!stream) {
        if (idx / 8 >= end) return 0;
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

Data DataSrc::readdata(const size_t size) {
    Data ret;
    ret.datasize = size;
    size_t i = 0;
    for (; i+8 <= size; i += 8)
        ret.data.push_back( (uint8_t)readint(8) );
    if (i < size) {
        auto x = size - i;
        auto v = (uint8_t)readint(x) << (8 - x);
        ret.data.push_back((uint8_t)v);
    }
    return ret;
}

uint64_t DataSrc::readint(size_t bits) {
    if (!stream) {
        uint64_t ret = 0;
        size_t i = 0;
        for (; i < bits and datacur % 8; i++, datacur++)
            ret = (ret << 1) | operator[](datacur);
        for (; i + 8 <= bits and datacur / 8 + 1 <= end; i += 8, datacur += 8)
            ret = (ret << 8) | data[datacur / 8];
        for (; i < bits; i++, datacur++)
            ret = (ret << 1) | operator[](datacur);
        return ret;
    } else {
        // TODO
        assert(!(bool)"TODO");
    }
}

