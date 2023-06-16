#!/bin/bash
set -eux

T=0
B=8
O=3
A='arithmetic-ppm'
name="$A.$B.$O"
ARGS="-t $A -v -b $B -n $O --no-time --no-skip"

DEBUG=1 make -j build
BIN=arithmetic-coding.exe

F="test.pth"
E="$F.${name}.enc"
D="$F.${name}.dec"

make -j build

"./bin/release/$BIN" \
  $ARGS -e \
  -i "$F" -o "$E" 2>debug-me.txt

"./bin/release/$BIN" \
  $ARGS -d \
  -i "$E" -o "$D"

if [ $? == 0 ]; then
  ls -l "$F" "$E" "$D"
  diff "$F" "$D" \
    && rm "$E" "$D" \
    || diff <(xxd "$F") <(xxd "$D")
fi
