#ifndef OPTIONS_H
#define OPTIONS_H

#define CAP_DEVICE_ID 0
#define DEV_WIDTH 320
#define TOLERANCE 1000
#define POLLING_CAP_INTERVAL 20
#define SAVE_IMG_ACTION 0
#define FILTER_TYPE 0
#define FILTER_VALUE 0
#define USE_GUI 1
#define CAPTURE_PATH "./capture/"
#define CAM_PREFIX "cam0_"

#include <verbosity.h>

typedef unsigned int ui_t;
typedef unsigned long int uli_t;
typedef struct cmd_options_s
{
    ui_t deviceId = CAP_DEVICE_ID;
    int threshold = TOLERANCE;
    ui_t cintval = POLLING_CAP_INTERVAL;
    ui_t width = DEV_WIDTH;
    ui_t verbosity = v_info;
    ui_t savimg = SAVE_IMG_ACTION;
    int filter_type = FILTER_TYPE;
    int filter_value = FILTER_VALUE;
    ui_t gui = USE_GUI;
    std::string cappath = CAPTURE_PATH;
    std::string prefix = CAM_PREFIX;
} cmd_options_t;

#endif // OPTIONS_H