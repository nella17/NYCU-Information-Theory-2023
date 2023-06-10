#include "coding-arithmetic-fpm.hpp"

#include "arithmetic-code.hpp"
#include "options.hpp"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>

namespace coding {

ArithmeticFPM::ArithmeticFPM(): Base() {}

ArithmeticFPM::~ArithmeticFPM() {}

size_t ArithmeticFPM::encode(DataSrc& src, DataDst& dst) {
    const size_t bits = opts.bits;
    const size_t charset = 1 << bits;
    auto origsize = src.total();
    auto size = origsize * 8 / bits;

    std::vector<uint32_t> prob(charset), fpm(charset), accum(charset+1);
    timer_start("calculate probability");
        src.reset();
        for (size_t i = 0; i < size; i++) {
            auto symbol = src.readint(bits);
            prob.at(symbol)++;
        }
        accum[0] = 0;
        for (size_t i = 0; i < charset; i++) {
            fpm.at(i) = prob.at(i);
            accum[i+1] = accum[i] + fpm[i];
        }
    timer_stop();

    Data data;
    Arithmetic code(data);
    timer_start_progress("compress file");
        src.reset();
        for (size_t i = 0; i < size; i++) {
            auto symbol = src.readint(bits);
            code.send(accum[symbol], accum[symbol+1], size);
            if (i % 10000 == 0)
                timer_progress((double)i / (double)size);
        }
    timer_stop_progress();

    timer_start("write file");
        for (size_t i = 0; i < charset; i++)
            dst.writeint(32, accum[i]);
        dst.writeint(32, origsize);
        dst.write(data);
        dst.write(true);
    timer_stop();

    auto compsize = (data.size() + 7) / 8;
    auto total_compsize = 4 * charset + 4 + compsize;
    if (opts.verbose) {
        std::cerr
            << "Original size    (split):   " << origsize << " bytes\n"
            << "Compressed size  (split):   " << compsize << " bytes (" << data.size() << " bits)\n"
            << "Compression ratio (split):  "
                << std::setw(5) << std::setprecision(2) << std::fixed
                << 100 * (double)((int32_t)origsize - (int32_t)compsize) / (double)origsize << "%\n"
            << std::flush;
    }

    return total_compsize;
}

size_t ArithmeticFPM::decode(DataSrc& src, DataDst& dst) {
    // TODO
}

}

