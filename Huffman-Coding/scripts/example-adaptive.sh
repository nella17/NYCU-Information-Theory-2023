#!/bin/bash
set -eux

# encode
./bin/release/huffman-coding.exe \
  -t adaptive -v -e \
  -i ./alexnet.pth -o "./alexnet.pth.adaptive.8.enc"
# decode
./bin/release/huffman-coding.exe \
  -t adaptive -v -d \
  -i "./alexnet.pth.adaptive.8.enc" -o "./alexnet.pth.adaptive.8.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.adaptive.8.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.adaptive.8.enc ./alexnet.pth.adaptive.8.dec
