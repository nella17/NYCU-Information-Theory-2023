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
#include "datasource.hpp"

#include "coding.hpp"

signed main(int argc, char* const argv[]) {
    opts.parse(argc, argv);

    auto func = coding::make();
    if (!func) USAGE();

    DataSource indata;
    std::string result;

    if (opts.encode) {
        result = func->encode(indata);
    } else {
        result = func->decode(indata);
    }

    return EXIT_SUCCESS;
}
