#!/bin/bash
set -eux
DEBUG=1 make -j build
./bin/debug/huffman-coding.exe -e -t adaptive -o /dev/null
