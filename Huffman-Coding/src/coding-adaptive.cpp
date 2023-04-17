#include "coding-adaptive.hpp"

#include "options.hpp"
#include "huffman-tree-fgk.hpp"
#include <iostream>
#include <iomanip>

namespace coding {

Adaptive::Adaptive(): Base() {}

Adaptive::~Adaptive() {}

#define DATA(func, ...) \
    do { \
        if (opts.ostream) \
            dst.func(__VA_ARGS__); \
        else \
            data.func(__VA_ARGS__); \
    } while (0)

size_t Adaptive::encode(DataSrc& src, DataDst& dst) {
    HuffmanTreeFGK ht(opts.bits);

    size_t compsize8 = 0;

    timer_start_progress("compress file");
    Data data;
    for (size_t cnt = 0; !src.eof(); cnt++) {
        auto value = src.readint(opts.bits);
        auto code = ht.encode(value);
        compsize8 += code.size();
        DATA(write, code);
        timer_progress((double)cnt / 1e8);
    }
    if (compsize8 % 8) {
        auto nytcode = ht.getnytcode();
        auto sz = 8 - compsize8 % 8;
        nytcode.resize(sz);
        DATA(write, nytcode);
    }
    timer_stop_progress();

    if (!opts.ostream) {
        timer_start("write file");
        dst.write(data);
        dst.write(false);
        timer_stop();
    }

    auto compsize = (compsize8 + 7) / 8;
    auto origsize = src.total();

    if (opts.verbose) {
        std::cerr
            << "Tree Height:        " << ht.height() << '\n'
            << "Original size:      " << origsize << " bytes\n"
            << "Compressed size:    " << compsize << " bytes (" << compsize8 << " bits)\n"
            << "Compression ratio:  "
                << std::setw(5) << std::setprecision(2) << std::fixed
                << 100 * (double)((int64_t)origsize - (int64_t)compsize) / (double)origsize << "%\n"
            << std::flush;
    }

    return compsize;
}

size_t Adaptive::decode(DataSrc& src, DataDst& dst) {
    HuffmanTreeFGK ht(opts.bits);

    size_t dsize = 0;

    timer_start_progress("decompress file");
    Data data;
    for (size_t cnt = 0; !src.eof(); cnt++) {
        auto value = ht.decode(src);
        if (value == (uint64_t)-1)
            break;
        dsize += opts.bits;
        DATA(writeint, opts.bits, value);
        timer_progress((double)cnt / 1e8);
    }
    timer_stop_progress();

    if (!opts.ostream) {
        timer_start("write file");
        dst.write(data);
        dst.write(false);
        timer_stop();
    }

    size_t compsize8 = src.total();
    auto compsize = (compsize8 + 7) / 8;

    if (opts.verbose) {
        std::cerr
            << "Tree size:          " << ht.height() << '\n'
            << "Compressed size:    " << compsize << " bytes (" << compsize8 << " bits)\n"
            << "Decompressed size:  " << dsize << " bytes\n"
            << std::flush;
    }

    return dsize;
}

}
