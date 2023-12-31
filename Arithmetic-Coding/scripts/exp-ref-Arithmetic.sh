#!/bin/bash
set -eux

mkdir log || true

exec > "log/exp.cmp.ref-Arithmetic.log" 2>&1

F=./alexnet.pth

/usr/bin/time -l -h -p \
  ../Reference-arithmetic-coding/cpp/ArithmeticCompress "$F" "$F.c"
/usr/bin/time -l -h -p \
  ../Reference-arithmetic-coding/cpp/ArithmeticDecompress "$F.c" "$F.c.d"

ls -l "$F" "$F.c" "$F.c.d"

diff "$F" "$F.c.d" \
  && echo "OK" \
  || echo "FAIL" \
  && diff <(xxd "$F") <(xxd "$F.c.d")
rm "$F.c" "$F.c.d"
