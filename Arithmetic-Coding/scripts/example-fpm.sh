#!/bin/bash
set -eux

# encode
./bin/release/arithmetic-coding.exe \
  -t arithmetic-fpm -b 8 -v -e \
  -i ./alexnet.pth -o "./alexnet.pth.fpm.8.enc"
# decode
./bin/release/arithmetic-coding.exe \
  -t arithmetic-fpm -b 8 -v -d \
  -i "./alexnet.pth.fpm.8.enc" -o "./alexnet.pth.fpm.8.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.fpm.8.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.fpm.8.enc ./alexnet.pth.fpm.8.dec
