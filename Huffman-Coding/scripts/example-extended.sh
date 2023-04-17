#!/bin/bash
set -eux

# encode
./bin/release/huffman-coding.exe \
  -t basic -b 32 -v -e \
  -i ./alexnet.pth -o "./alexnet.pth.basic.32.enc"
# decode
./bin/release/huffman-coding.exe \
  -t basic -b 32 -v -d \
  -i "./alexnet.pth.basic.32.enc" -o "./alexnet.pth.basic.32.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.basic.32.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.basic.32.enc ./alexnet.pth.basic.32.dec
