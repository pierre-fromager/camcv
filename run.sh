#!/bin/bash

if [ -d "./build" ]; then
    ./build/camcv --d 0 --t 1000 --w 320 --v 0 --i 2 --s 1
fi
