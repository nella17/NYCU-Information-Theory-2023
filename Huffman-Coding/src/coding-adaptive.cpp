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
        uint64_t value = src.readint(opts.bits);
        auto code = ht.encode(value);
        compsize8 += code.size();
        dst.write(code);
        timer_progress(double(cnt));
    }
    timer_stop_progress();

    auto compsize = (compsize8 + 7) / 8;
    auto origsize = src.total();

    if (opts.verbose) {
        std::cerr
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
    // TODO
}

}
