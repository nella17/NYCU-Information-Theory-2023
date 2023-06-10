#!/bin/bash
set -x
if [ ! -n "$ID" ]; then
  exit -1;
fi

rm -fr "$ID.zip"
mkdir "$ID"
cp -r src scripts Makefile README.md "$ID"
zip -r "$ID.zip" "$ID" -x "*/.DS_Store" -x "__MACOSX"
rm -fr "$ID"
cp report/report.pdf "$ID.pdf"
