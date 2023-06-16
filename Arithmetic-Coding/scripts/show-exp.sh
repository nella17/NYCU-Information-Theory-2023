#!/bin/bash
for f in ./log/*.log; do
  s=$(grep -E 'real' $f | awk '{ print $2 }')
  tc=$(echo $s | awk '{ print $1 }')
  td=$(echo $s | awk '{ print $2 }')
  s=$(grep -E 'peak' $f | awk '{ print $1 }')
  mc=$(echo $s | awk '{ print $1 }')
  md=$(echo $s | awk '{ print $2 }')
  echo -e "$tc\t$mc\t$td\t$md" "\t" $f
done
