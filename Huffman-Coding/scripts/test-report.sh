#!/bin/bash
set -eux

/usr/bin/time -l -h -p ./bin/release/huffman-coding.exe \
  $ARGS -e \
  -i ./alexnet.pth -o "./alexnet.pth.${name}.enc"

/usr/bin/time -l -h -p ./bin/release/huffman-coding.exe \
  $ARGS -d \
  -i "./alexnet.pth.${name}.enc" -o "./alexnet.pth.${name}.dec"
if [ $? == 0 ]; then
  diff ./alexnet.pth "./alexnet.pth.${name}.dec" \
    && rm "./alexnet.pth.${name}.enc" "./alexnet.pth.${name}.dec"
fi
