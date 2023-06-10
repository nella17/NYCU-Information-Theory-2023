#include "options.hpp"

#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

Options opts;

const char usage[] = R"(
Usage: %s -t <type> [-e | -d] [options...] [-i <file>] [-o <file>]
    -t, --type <type>       Set coding algorithm
    -e, --encode            Encode
    -d, --decode            Decode
    -i, --input <file>      Set input file (default STDIN)
    -o, --output <file>     Set output file (default STDOUT)
    --ostream               Output stream
    -b, --bits <bits>       Tread input file as <bits> data source (default 8)
    -s, --split <size>      Split input file every <size> bytes of data (default ∞)
    -v, --verbose           Show debug / analysis /time info
    --pmf                   Show pmf freq
    --no-time               No show time info
    -h, --help              Show this help

Coding Algorithms
    analysis    Analysis entropy
    basic       Basic Huffman Coding Algorithm (bits: 8 <= 8k <= 64)
    adaptive    Adaptive Huffman Coding Algorithm
    extended    Extended Huffman Coding Algorithm (TODO)
)";

const char optstring[] = "t:edi:o:b:s:vh";
const struct option longopts[] = {
    { "type",   required_argument,  0,  't' },
    { "encode", no_argument,        0,  'e' },
    { "decode", no_argument,        0,  'd' },
    { "input",  required_argument,  0,  'i' },
    { "output", required_argument,  0,  'o' },
    { "ostream",no_argument,        &opts.ostream,  1 },
    { "bits",   required_argument,  0,  'b' },
    { "split",  required_argument,  0,  's' },
    { "verbose",no_argument,        0,  'v' },
    { "pmf",    no_argument,        &opts.pmf,      1  },
    { "no-time",no_argument,        &opts.notime,   1  },
    { "help",   no_argument,        0,  'h' },
    { 0,        0,                  0,   0  },
};


void Options::parse(int argc, char* const argv[]) {
    while (1) {
        int optidx = 0;
        int c = getopt_long(argc, argv, optstring, longopts, &optidx);
        if (c == -1)
            break;

        switch (c) {
            case 0:
                break;

            case 't':
                type = optarg;
                if (type == "basic")
                    stream = false;
                break;

            case 'e':
                encode = true;
                break;

            case 'd':
                decode = true;
                break;

            case 'i':
                if (input_fd != STDIN_FILENO)
                    close(input_fd);
                input_fd = open(optarg, O_RDONLY);
                if (input_fd < 0) perror("open(input)"), exit(EXIT_FAILURE);
                break;

            case 'o':
                if (output_fd != STDOUT_FILENO)
                    close(output_fd);
                output_fd = open(optarg, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                if (output_fd < 0) perror("open(output)"), exit(EXIT_FAILURE);
                break;

            case 'b':
                bits = (size_t)atoi(optarg);
                break;

            case 's':
                split = (size_t)atoi(optarg);
                break;

            case 'v':
                verbose = true;
                notime = 0;
                break;

            case 'h':
            default:
                USAGE();
        }
    }

    bool check = true;
    check &= !type.empty();
    check &= type == "analysis" or encode ^ decode;
    check &= type != "basic" or (bits % 8 == 0 and stream == 0);
    check &= split == INF_SIZET or split % bits == 0;
    if (!check) USAGE();
}
