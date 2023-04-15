#!/bin/bash
set -eux

. config.sh

DEBUG=1 make -j build
make -j build

set +e

./bin/debug/huffman-coding.exe \
  $ARGS -d \
  -i "./test.png.${name}.enc" -o "./test.png.${name}.dec"
if [ $? == 0 ]; then
  diff  ./test.png "./test.png.${name}.dec" \
    && rm "./test.png.${name}.enc" "./test.png.${name}.dec"
fi

if [ "$T" == 1 ]; then
  exit 0
fi

echo

./bin/release/huffman-coding.exe \
  $ARGS -d \
  -i "./alexnet.pth.${name}.enc" -o "./alexnet.pth.${name}.dec"
if [ $? == 0 ]; then
  diff ./alexnet.pth "./alexnet.pth.${name}.dec" \
    && rm "./alexnet.pth.${name}.enc" "./alexnet.pth.${name}.dec"
fi
