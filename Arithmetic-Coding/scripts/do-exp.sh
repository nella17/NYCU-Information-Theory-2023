#!/bin/bash
set -x

mkdir log

if [ "$1" = 1 ]; then
  ./scripts/exp.sh
fi

for f in ./scripts/exp-*.sh; do
  $f
done
