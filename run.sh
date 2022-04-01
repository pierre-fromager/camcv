#!/bin/bash

if [ -d "./build" ]; then
    ./build/camcv --d 0 --t 1000 --w 320 --v 1 --i 2
fi
