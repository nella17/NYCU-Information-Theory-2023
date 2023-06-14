#!/bin/bash
set -ux

mkdir log

for k in 1 8; do
  (name="fpm.$k" ARGS="-t arithmetic-fpm -b $k -v --no-time" ./scripts/test-report.sh > "log/exp.fpm.$k.log" 2>&1)
  for j in {-1..3}; do
    (name="ppm.$k.$j" ARGS="-t arithmetic-ppm -b $k -n $j -v --no-time" ./scripts/test-report.sh > "log/exp.ppm.$k.$j.log" 2>&1)
  done
done
