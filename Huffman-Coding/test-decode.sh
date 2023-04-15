#!/bin/bash
set -eux

. config.sh

DEBUG=1 make -j build
make -j build

./bin/debug/huffman-coding.exe \
  $ARGS \
  -i "./test.png.${name}.enc" -o "./test.png.${name}.dec"

./bin/release/huffman-coding.exe \
  $ARGS \
  -i "./alexnet.pth.${name}.enc" -o "./alexnet.path.${name}.dec"
