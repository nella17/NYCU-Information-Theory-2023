#pragma once

#include <getopt.h>
#include <string>

#define USAGE() \
    do { \
        fprintf(stderr, usage, argv[0]); \
        exit(EXIT_FAILURE); \
    } while (0)

extern const char usage[];

class Options {
public:
    std::string type;
    bool encode = false, decode = false, stream = true, verbose = false;
    int input_fd = STDIN_FILENO, output_fd = STDOUT_FILENO;
    size_t bits = 8;

    void parse(int argc, char* const argv[]);
};
