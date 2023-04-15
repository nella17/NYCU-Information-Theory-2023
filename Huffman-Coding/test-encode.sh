#!/bin/bash
set -eux

. config.sh

DEBUG=1 make -j build
make -j build

./bin/debug/huffman-coding.exe \
  $ARGS -e \
  -i ./test.png -o "./test.png.${name}.enc"

./bin/release/huffman-coding.exe \
  $ARGS -e \
  -i ./alexnet.pth -o "./alexnet.pth.${name}.enc"
