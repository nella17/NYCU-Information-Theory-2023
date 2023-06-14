#!/bin/bash
set -x

mkdir log

for f in ./scripts/exp-*.sh; do
  $f
done
