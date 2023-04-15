#!/bin/bash
set -eux

ARGS="-t basic -d -b 32"
name="basic"

DEBUG=1 make -j build
make -j build

./bin/debug/huffman-coding.exe \
  $ARGS \
  -i "./test.png.${name}.enc" -o "./test.png.${name}.dec"

./bin/release/huffman-coding.exe \
  $ARGS \
  -i "./alexnet.pth.${name}.enc" -o "./alexnet.path.${name}.dec"
