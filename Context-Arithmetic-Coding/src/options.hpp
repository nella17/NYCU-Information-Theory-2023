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
    std::string type, model;
    bool encode = false, decode = false, verbose = false, stream = false;
    int notime = 1, pmf = 0, ostream = 0;
    int input_fd = STDIN_FILENO, output_fd = STDOUT_FILENO;
    size_t bits = 8, order = 2;

    void parse(int argc, char* const argv[]);
};

extern Options opts;
