
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <string>
#include <ctime>
#include <opencv4/opencv2/opencv.hpp>

#define DEBUG 1
#define MAIN_MSG "Press ESC to exit"
#define WINDOW_TITLE "Motion detect"
#define ERR_MSG "Can't open device"
#define CAP_DEVICE_ID 0
#define POLLING_KEY_TIME 1
#define POLLING_CAP_INTERVAL 10
#define ESC_CODE 27
#define DEV_WIDTH 320
#define SPACE " "
#define LINE_FEED '\n'
#define DIFF_WEIGHT_LABEL "Difs:"
#define CAPTURE_LABEL "Capture"
#define PCT_TOLERANCE 10

#endif // CONFIG_H
