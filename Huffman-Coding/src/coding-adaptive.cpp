#include "coding-adaptive.hpp"

#include "options.hpp"
#include "huffman-tree-fgk.hpp"
#include <iostream>
#include <iomanip>

namespace coding {

Adaptive::Adaptive(): Base() {}

Adaptive::~Adaptive() {}

size_t Adaptive::encode(DataSrc& src, DataDst& dst) {
    HuffmanTreeFGK ht(opts.bits);

    size_t compsize8 = 0;

    timer_start_progress("compress file");
    for (size_t cnt = 0; !src.eof(); cnt++) {
        auto value = src.readint(opts.bits);
        auto code = ht.encode(value);
        compsize8 += code.size();
        dst.write(code);
        timer_progress((double)cnt / 1e8);
    }
    timer_stop_progress();

    auto compsize = (compsize8 + 7) / 8;
    auto origsize = src.total();

    if (opts.verbose) {
        std::cerr
            << "Tree Height: " << ht.height() << '\n'
            << "Original size: " << origsize << " bytes\n"
            << "Compressed size (file): " << compsize << " bytes (" << compsize8 << " bits)\n"
            << "Compression rate (file): "
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
        data.writeint(opts.bits, value);
        timer_progress((double)cnt / 1e8);
    }
    timer_stop_progress();

    timer_start("write file");
    dst.write(data);
    dst.write(false);
    timer_stop();

    size_t compsize8 = src.total();
    auto compsize = (compsize8 + 7) / 8;

    if (opts.verbose) {
        std::cerr
            << "Tree size: " << ht.height() << '\n'
            << "Decompressed size: " << dsize << " bytes\n"
            << std::flush;
    }

    return dsize;
}

}
