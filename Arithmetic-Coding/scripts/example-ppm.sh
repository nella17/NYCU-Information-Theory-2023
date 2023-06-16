#!/bin/bash
set -eux

# encode
./bin/release/arithmetic-coding.exe \
  -t arithmetic-ppm -b 8 -n 1 -v -e \
  -i ./alexnet.pth -o "./alexnet.pth.ppm.8.1.enc"
# decode
./bin/release/arithmetic-coding.exe \
  -t arithmetic-ppm -b 8 -n 1 -v -d \
  -i "./alexnet.pth.ppm.8.1.enc" -o "./alexnet.pth.ppm.8.1.dec"
# verify
diff ./alexnet.pth ./alexnet.pth.ppm.8.1.dec && echo "OK" || echo "Failed"
# cleanup
rm ./alexnet.pth.ppm.8.1.enc ./alexnet.pth.ppm.8.1.dec
