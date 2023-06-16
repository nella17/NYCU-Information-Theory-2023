#!/bin/bash
set -x
if [ ! -n "$ID" ]; then
  exit -1;
fi

rm -fr "$ID.zip"
mkdir "$ID" "$ID/scripts"
cp -r src Makefile README.md "$ID"
cp scripts/exp.sh scripts/test-report.sh scripts/example* "$ID/scripts"
zip -r "$ID.zip" "$ID" -x "*/.DS_Store" -x "__MACOSX"
rm -fr "$ID"
cp report/report.pdf "$ID.pdf"
