#pragma once

#include <unistd.h>
#include <getopt.h>
#include <string>
#include <limits>

#include "utils.hpp"

#define USAGE() \
    do { \
        fprintf(stderr, usage, argv[0]); \
        exit(EXIT_FAILURE); \
    } while (0)

extern const char usage[];

class Options {
public:
    std::string type;
    bool encode = false, decode = false, verbose = false;
    int notime = 1, stream = 0, pmf = 0;
    int input_fd = STDIN_FILENO, output_fd = STDOUT_FILENO;
    size_t bits = 8, split = INF_SIZET;

    void parse(int argc, char* const argv[]);
};

extern Options opts;
