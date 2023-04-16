#include <unistd.h>
#include <stdio.h>

#include <iostream>
#include <fstream>
#include <unordered_map>

#include "options.hpp"
#include "utils.hpp"
#include "datasrc.hpp"
#include "coding.hpp"

signed main(int argc, char* const argv[]) {
    opts.parse(argc, argv);

    if (opts.type == "analysis") {
        opts.notime = 0;

        std::unordered_map<uint64_t, size_t> map{};
        DataSrc src(false, opts.input_fd);
        size_t origsize = src.size() / 8;
        size_t total = src.size() / opts.bits;

        timer_start_progress("calculate pmf");
        for (size_t cnt = 0; !src.eof(); cnt++) {
            uint64_t value = src.readint(opts.bits);
            map[value] += 1;
            timer_progress(double(cnt) / double(total));
        }
        timer_stop_progress();

        std::vector<std::pair<size_t, uint64_t>> freq;
        freq.reserve(map.size());
        for (auto [v, c]: map)
            freq.emplace_back(c, v);

        auto entropy = calc_entropy(freq);
        std::cerr
            << "Entropy:            " << entropy << '\n'
            << std::flush;

        return EXIT_SUCCESS;
    }

    auto func = coding::make();
    if (!func) USAGE();

    DataSrc src(opts.stream, opts.input_fd);
    DataDst dst(opts.output_fd);

    size_t origsize, compsize;

    if (opts.encode) {
        compsize = 0;
        src.resplit(opts.split);
        do {
            compsize += func->encode(src, dst);
        } while (src.nextsplit());
        origsize = src.total();
    } else {
        origsize = 0;
        do {
            origsize += func->decode(src, dst);
        } while (!src.eof());
        compsize = src.total();
    }

    std::cerr
        << "Compressed size:        " << compsize << " bytes\n"
        << "Original size:          " << origsize << " bytes\n"
        << "Compression rate:       "
            << std::setw(5) << std::setprecision(2) << std::fixed
            << 100 * (double)((int64_t)origsize - (int64_t)compsize) / (double)origsize << "%\n"
        << "Expected codeword length (include header): "
            << std::setw(5) << std::setprecision(2) << std::fixed
            << (double)compsize / ((double)origsize / (double)opts.bits) << "\n"
        << std::flush;

    return EXIT_SUCCESS;
}
