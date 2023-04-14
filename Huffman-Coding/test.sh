#!/bin/bash
set -eux
CXX=g++-12 DEBUG=1 make -j build
./bin/debug/huffman-coding.exe \
  -t basic -e -b 8 \
  -i ./alexnet.pth -o ./alexnet.path.basic.enc
