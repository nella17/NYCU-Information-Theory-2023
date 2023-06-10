#include "coding-arithmetic-fpm.hpp"

#include "arithmetic-code.hpp"
#include "options.hpp"
#include <algorithm>
#include <numeric>
#include <iostream>
#include <iomanip>

constexpr size_t STEP = 10000;

namespace coding {

void ArithmeticFPM::build() {
    std::iota(i2s.begin(), i2s.end(), 0);
    std::stable_sort(i2s.begin(), i2s.end(), [&](auto a, auto b) {
        return prob[a] > prob[b];
    });
    accum[0] = 0;
    for (size_t i = 0; i <= charset; i++) {
        auto symbol = i2s[i];
        accum[i+1] = accum[i] + prob[symbol];
        s2i[symbol] = (uint32_t)i;
    }
}

ArithmeticFPM::ArithmeticFPM(): Base(),
    bits(opts.bits), charset(1 << opts.bits),
    prob(charset+1), accum(charset+2), s2i(charset+1), i2s(charset+1) {
    prob[charset]++;
}

ArithmeticFPM::~ArithmeticFPM() {}

size_t ArithmeticFPM::encode(DataSrc& src, DataDst& dst) {
    auto origsize = src.total();
    auto size = origsize * 8 / bits;

    timer_start("calculate probability");
        src.reset();
        for (size_t i = 0; i < size; i++) {
            auto symbol = src.readint(bits);
            prob.at(symbol)++;
        }
        build();
    timer_stop();

    Data data;
    Arithmetic code;
    timer_start_progress("compress file");
        src.reset();
        for (size_t i = 0; i <= size; i++) {
            auto symbol = i == size ? charset : (uint32_t)src.readint(bits);
            auto idx = s2i[symbol];
            auto bs = code.send(accum, idx);
            data.write(bs);
            if (i % STEP == 0)
                timer_progress((double)i / (double)size);
        }
    timer_stop_progress();

    timer_start("write file");
        dst.writeint(32, origsize);
        for (size_t i = 0; i < charset; i++)
            dst.writeint(32, prob[i]);
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
    auto total_compsize = src.total();
    auto compsize = total_compsize - 4 * charset - 4;

    auto origsize = src.readint(32);
    auto size = origsize * 8 / bits;

    timer_start("calculate probability");
        for (size_t i = 0; i < charset; i++)
            prob[i] = (uint32_t)src.readint(32);
        build();
    timer_stop();

    Data data;
    Arithmetic code;
    timer_start_progress("decompress file");
        code.T = src.readint(code.BITS);
        for (size_t i = 0; i < size; i++) {
            auto idx = code.recv(src, accum);
            auto symbol = i2s[idx];
            data.writeint(bits, symbol);
            if (i % STEP == 0)
                timer_progress((double)i / (double)size);
        }
    timer_stop_progress();

    timer_start("write file");
        dst.write(data);
        dst.write(true);
    timer_stop();

    auto dsize = data.size() / 8;
    if (opts.verbose) {
        std::cerr
            << "Compressed size:    " << compsize << " bytes (" << src.total() << " bits)\n"
            << "Decompressed size:  " << dsize << " bytes\n"
            << std::flush;
    }

    return dsize;
}

}

