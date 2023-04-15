#include <unistd.h>
#include <stdio.h>

#include <iostream>
#include <fstream>

#ifndef MAX_FILE_SIZE
// 512MB
#define MAX_FILE_SIZE (1024 * 1024 * 512)
#endif

#include "options.hpp"
#include "utils.hpp"
#include "datasrc.hpp"
#include "coding.hpp"

signed main(int argc, char* const argv[]) {
    opts.parse(argc, argv);

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
        } while (src.remain() >= 64);
        compsize = src.total();
    }

    std::cerr
        << "Compressed size: " << compsize << " bytes\n"
        << "Original size: " << origsize << " bytes\n"
        << "Compression rate: "
            << std::setw(5) << std::setprecision(2) << std::fixed
            << 100 * (double)((int64_t)origsize - (int64_t)compsize) / (double)origsize << "%\n"
        << std::flush;

    return EXIT_SUCCESS;
}
