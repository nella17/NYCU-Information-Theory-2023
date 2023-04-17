#!/bin/bash
set -eux

# encode
./bin/release/huffman-coding.exe \
  -t basic -b 8 -v -e -s 41943040 \
  -i ./alexnet.pth -o "./alexnet.pth.basic.8.s40MB.enc"
# decode
./bin/release/huffman-coding.exe \
  -t basic -b 8 -v -d -s 41943040 \
  -i "./alexnet.pth.basic.8.s40MB.enc" -o "./alexnet.pth.basic.8.s40MB.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.basic.8.s40MB.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.basic.8.s40MB.enc ./alexnet.pth.basic.8.s40MB.dec
