#!/bin/bash
set -eux

T=1
B=8
A='arithmetic-ppm'
name="$A.$B"
ARGS="-t $A -v -b $B -n 2"

DEBUG=1 make -j build
BIN=arithmetic-coding.exe

time "./bin/debug/$BIN" \
  $ARGS -e \
  -i ./test.png -o "./test.png.${name}.enc"

time "./bin/debug/$BIN" \
  $ARGS -d \
  -i "./test.png.${name}.enc" -o "./test.png.${name}.dec"
if [ $? == 0 ]; then
  diff  ./test.png "./test.png.${name}.dec" \
    && rm "./test.png.${name}.enc" "./test.png.${name}.dec"
fi

if [ "$T" == 1 ]; then
  exit 0
fi

make -j build

time "./bin/release/$BIN" \
  $ARGS -e \
  -i ./alexnet.pth -o "./alexnet.pth.${name}.enc"

time "./bin/release/$BIN" \
  $ARGS -d \
  -i "./alexnet.pth.${name}.enc" -o "./alexnet.pth.${name}.dec"
if [ $? == 0 ]; then
  diff ./alexnet.pth "./alexnet.pth.${name}.dec" \
    && rm "./alexnet.pth.${name}.enc" "./alexnet.pth.${name}.dec"
fi
