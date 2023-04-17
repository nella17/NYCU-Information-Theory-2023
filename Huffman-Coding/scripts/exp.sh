#!/bin/bash
set -ux

for i in {1..8}; do
  k="$((i * 8))"
  (name="basic.$k" ARGS="-t basic -b $k -v --no-time" ./scripts/test-report.sh > "log/exp-basic-$k.log" 2>&1)
  (name="basic.$k.40MB" ARGS="-t basic -b $k -s 41943040 -v --no-time" ./chart-compression-ratio-split/test-report.sh > "log/exp-basic-$k-40MB.log" 2>&1)
done

(name="adaptive.8" ARGS="-t adaptive -b 8 -v --no-time" ./chart-compression-ratio-split/test-report.sh > "log/exp-adaptive-8.log" 2>&1)

time diff alexnet.pth <(./bin/release/huffman-coding.exe -t adaptive -v -e -i ./alexnet.pth --ostream | ./bin/release/huffman-coding.exe -t adaptive -v -d) && echo "success"

wait
