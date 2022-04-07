# CamCV

Motion detect on webcam.  
Learn differences from previous and current image then trigger action when diff ratio greater than tolerance.  

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
Identify cam devices
``` 
#!/bin/bash

for f in /dev/video*; do
    item="$(basename -- $f)"
    name="$(cat /sys/class/video4linux/$item/name)"
    echo "$f  =>  $name"
done
``` 
## Screenshot

![sc](doc/assets/screenshot.jpeg)

## Usage

``` 
./run.sh
```
Display help
``` 
./build/camcv --help

Usage camcv [options]:
  --help                display options
  --d arg               device id
  --t arg               threshold
  --w arg               width
  --v arg               verbosity
  --i arg               capture interval
  --s arg               save image
  --ft arg              filter type
  --fv arg              filter value
  --gui arg             use GUI
  --cap arg             capture Path
```
Sample run options
```
./build/camcv --d 0 --t 1000 --w 320 --v 1 --i 2 --s 1
```
In this case we :
* use first video device (/dev/video0)
* set threshold with 1000 as diff value from compare
* set capture dimension to 320x240
* enable verbosity as info
* capture every 2 frames
* save image on motion detect

### Gui
* 'm' toggle diff/real mode.
* Filter type [0..4] (Binary, Binary Inverted, Truncate, To Zero, Zero Inverted)
* Filter value

## Perfomances

Check Cpu load
```
top -c -p $(pgrep -d',' -f camcv)
```

* On i5-3320M, 320x240, filter activated, without gui, load is 2.7% per instance
* Consider file saving as greedy (+2% cpu load)

## Capture consolidate
You can use ffmpeg to consolidate all capture files into a single movie.

```
ffmpeg -framerate 10 -pattern_type glob -i '*.jpg' -c:v libx264 -pix_fmt yuv420p consolidate.mp4
```

## Looking further
* Zoning [there](https://github.com/cedricve/motion-detection)
* [OpenCV](https://github.com/joachimBurket/esp32-opencv) on esp32
