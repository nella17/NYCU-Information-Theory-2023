#!/bin/bash
set -eux

# encode and decode
./bin/release/huffman-coding.exe \
  -t adaptive -v --no-time -e \
  -i ./alexnet.pth --ostream \
  | \
./bin/release/huffman-coding.exe \
  -t adaptive -v -d -o \
  ./alexnet.pth.adaptive.ostream --ostream
# verify
diff ./alexnet.pth ./alexnet.pth.adaptive.ostream && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.adaptive.ostream
