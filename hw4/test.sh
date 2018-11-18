#!/bin/bash

tmp=$(mktemp -d)
cd $tmp
touch -d '1918-11-11 11:00 GMT' wwi-armistice
touch now
sleep 1
touch now1
TZ=UTC0 ~/Desktop/CS35L/hw4/coreutils-with-bug/src/ls -lt --full-time wwi-armistice now now1
cd
# rm -fr $tmp
