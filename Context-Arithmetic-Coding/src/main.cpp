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

    auto func = coding::make();
    if (!func) USAGE();

    DataSrc src(opts.stream, opts.input_fd);
    DataDst dst(opts.output_fd);

    size_t origsize, compsize;

    if (opts.encode) {
        compsize = func->encode(src, dst);
        origsize = src.total();
    } else {
        origsize = func->decode(src, dst);
        compsize = src.total();
    }

    std::cerr
        << "Compressed size:        " << compsize << " bytes\n"
        << "Original size:          " << origsize << " bytes\n"
        << "Compression ratio:      "
            << std::setw(5) << std::setprecision(2) << std::fixed
            << 100 * (double)((int64_t)origsize - (int64_t)compsize) / (double)origsize << "%\n"
        << "Expected codeword length (include header): "
            << std::setw(5) << std::setprecision(2) << std::fixed
            << (double)compsize / ((double)origsize / (double)opts.bits) << "\n"
        << std::flush;

    return EXIT_SUCCESS;
}
