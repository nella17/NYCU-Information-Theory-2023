#!/bin/bash
set -eux

# encode
./bin/release/huffman-coding.exe \
  -t basic -b 8 -v -e \
  -i ./alexnet.pth -o "./alexnet.pth.basic.8.enc"
# decode
./bin/release/huffman-coding.exe \
  -t basic -b 8 -v -d \
  -i "./alexnet.pth.basic.8.enc" -o "./alexnet.pth.basic.8.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.basic.8.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.basic.8.enc ./alexnet.pth.basic.8.dec
