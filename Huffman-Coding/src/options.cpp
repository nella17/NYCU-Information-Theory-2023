#include "options.hpp"

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

const char usage[] = R"(
Usage: %s -t <type> [-e | -d] [options...] [-i <file>] [-o <file>]
    -t, --type <type>       Set coding algorithm
    -e, --encode            Encode
    -d, --decode            Decode
    -i, --input <file>      Set input file (default STDIN)
    -o, --output <file>     Set output file (default STDOUT)
    -b, --bits <bits>       Tread input file as <bits> data source (default 8) (mutiple of 8 only)

Coding Algorithms
    basic       Basic Huffman Coding Algorithm
    adaptive    Adaptive Huffman Coding Algorithm
    extended    Extended Huffman Coding Algorithm
)";

const char optstring[] = "t:edi:o:b:";
const struct option longopts[] = {
    { "type",   required_argument,  0,  't' },
    { "encode", no_argument,        0,  'e' },
    { "decode", no_argument,        0,  'd' },
    { "input",  required_argument,  0,  'i' },
    { "output", required_argument,  0,  'o' },
    { "bits",   required_argument,  0,  'b' },
    { 0,        0,                  0,   0  },
};


void Options::parse(int argc, char* const argv[]) {
    while (1) {
       int optidx = 0;
       int c = getopt_long(argc, argv, optstring, longopts, &optidx);
       if (c == -1)
           break;

       switch (c) {
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
           if (bits % 8) USAGE();
           break;

       default:
           USAGE();
       }
    }

    if (!(encode ^ decode) or type.empty()) USAGE();
}
