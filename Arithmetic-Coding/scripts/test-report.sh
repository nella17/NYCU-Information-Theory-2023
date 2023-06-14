#!/bin/bash
set -eux

BIN=arithmetic-coding.exe

F=./alexnet.pth
E="./alexnet.pth.${name}.enc"
D="./alexnet.pth.${name}.dec"

/usr/bin/time -l -h -p ./bin/release/$BIN \
  $ARGS -e \
  -i "$F" -o "$E"

/usr/bin/time -l -h -p ./bin/release/$BIN \
  $ARGS -d \
  -i "$E" -o "$D"

s=$?

ls -l "$F" "$E" "$D"

if [ $s == 0 ]; then
  diff "$F" "$D" \
    && rm "$E" "$D"  \
    || diff <(xxd "$F") <(xxd "$D")
fi
