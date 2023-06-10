#include "coding-arithmetic-fpm.hpp"

#include "arithmetic-code.hpp"
#include "options.hpp"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>

constexpr size_t STEP = 100000;

namespace coding {

ArithmeticFPM::ArithmeticFPM(): Base() {}

ArithmeticFPM::~ArithmeticFPM() {}

size_t ArithmeticFPM::encode(DataSrc& src, DataDst& dst) {
    const size_t bits = opts.bits;
    const size_t charset = 1 << bits;
    auto origsize = src.total();
    auto size = origsize * 8 / bits;

    std::vector<uint32_t> prob(charset), accum(charset+1);
    timer_start("calculate probability");
        src.reset();
        for (size_t i = 0; i < size; i++) {
            auto symbol = src.readint(bits);
            prob.at(symbol)++;
        }
        accum[0] = 0;
        for (size_t i = 0; i < charset; i++)
            accum[i+1] = accum[i] + prob[i];
    timer_stop();

    Data data;
    Arithmetic code(accum);
    timer_start_progress("compress file");
        src.reset();
        for (size_t i = 0; i < size; i++) {
            auto symbol = (uint32_t)src.readint(bits);
            auto bs = code.send(symbol);
            data.write(bs);
            if (i % STEP == 0)
                timer_progress((double)i / (double)size);
        }
    timer_stop_progress();

    timer_start("write file");
        dst.writeint(32, origsize);
        for (size_t i = 0; i < charset; i++)
            dst.writeint(32, accum[i]);
        dst.write(data);
        dst.write(true);
    timer_stop();

    auto compsize = (data.size() + 7) / 8;
    auto total_compsize = 4 + 4 * charset + compsize;
    if (opts.verbose) {
        std::cerr
            << "Original size:      " << origsize << " bytes\n"
            << "Compressed size:    " << compsize << " bytes (" << data.size() << " bits)\n"
            << "Compression ratio:  "
                << std::setw(5) << std::setprecision(2) << std::fixed
                << 100 * (double)((int64_t)origsize - (int64_t)compsize) / (double)origsize << "%\n"
            << std::flush;
    }

    return total_compsize;
}

size_t ArithmeticFPM::decode(DataSrc& src, DataDst& dst) {
    // TODO
}

}

