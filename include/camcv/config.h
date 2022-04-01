
#ifndef CONFIG_H
#define CONFIG_H

#include <stdio.h>
#include <string>
#include <opencv4/opencv2/opencv.hpp>

#define MAIN_MSG "Press ESC to exit"
#define WINDOW_TITLE "Motion detect"
#define ERR_MSG "Can't open device"
#define POLLING_KEY_TIME 1
#define ESC_CODE 27
#define KEY_MODE 'm'
#define SPACE " "
#define JPEG_EXT ".jpg"
#define DIFF_WEIGHT_LABEL "Difs:"
#define MOUSE_EVENT "Mouse event "
#define CAPTURE_LABEL "Capture"
#define MOTION_DETECTED_LABEL "MotionWeight:"
#define SMOTION_DETECTED_LABEL SPACE MOTION_DETECTED_LABEL

#endif // CONFIG_H
