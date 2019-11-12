#!/bin/bash

tmp1=$(mktemp)
tmp2=$(mktemp)

cat sample.json | jq -S . >> $tmp1
./a.out | jq -S . >> $tmp2
df=`diff $tmp1 $tmp2`

rm -f $tmp1
rm -f $tmp2

if [ "x$df" != "x" ]; then
  echo "faile to parse json"
  exit 1
fi

echo "ok"
