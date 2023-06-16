#!/bin/bash
set -eux

F=test.pth

make -C ../Reference-arithmetic-coding/cpp

../Reference-arithmetic-coding/cpp/PpmCompress "$F" "$F.c" 2>debug-ppm.txt

../Reference-arithmetic-coding/cpp/PpmDecompress "$F.c" "$F.c.d" 2>/dev/null

ls -l "$F" "$F.c" "$F.c.d"

diff "$F" "$F.c.d" \
  && echo "OK" \
  || echo "FAIL" \
  && diff <(xxd "$F") <(xxd "$F.c.d")
rm "$F.c" "$F.c.d"
