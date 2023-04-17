#!/bin/bash
set -eux

# encode and decode and verify
./bin/release/huffman-coding.exe \
  -t adaptive -v --no-time -e \
  -i ./alexnet.pth --ostream \
  | \
./bin/release/huffman-coding.exe \
  -t adaptive -v -d  --ostream \
  | \
diff ./alexnet.pth /dev/stdin \
  && echo "OK" || echo "Failed"
