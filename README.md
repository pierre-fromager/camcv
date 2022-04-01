# CamCV

Motion detect on webcam using openCV.  
Learning differences from previous and current image and trigger action when diff ratio greater than tolerance.  

## Requirements

* Boost lib (v1.78.0)
* OpenCV lib (v4.x)
* C++ compiler (v10.2.1)

## Setup

### Debian
``` 
sudo apt install cmake libboost-all-dev libopencv-dev
```

## Build
``` 
./build.sh
```

## Run
``` 
./run.sh
```

## Devices
To identify devices use the snippet below
``` 
#!/bin/bash

for f in /dev/video*; do
    item="$(basename -- $f)"
    name="$(cat /sys/class/video4linux/$item/name)"
    echo "$f  =>  $name"
done
``` 

## Usage

Simply
``` 
./run.sh
```
Or ask for help
``` 
./build/camcv --help

Usage camcv [options]:
  --help                help options
  --d arg               set device id
  --t arg               set threshold
  --w arg               set width
  --v arg               set verbosity
```
With options
```
./build/camcv --d 0 --t 1000 --w 320 --v 1
```
### Keys
* 'm' toggle diff mode.

## Going further
Look [there](https://github.com/cedricve/motion-detection) to implement zoning method

## Todo

* Implement zoning