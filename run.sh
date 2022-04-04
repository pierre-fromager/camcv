#!/bin/bash

if [ -d "./build" ]; then
    ./build/camcv --d 0 --t 2000 --w 320 --v 2 --i 2 --ft 1 --fv 120 --s 0 --gui 1
fi
