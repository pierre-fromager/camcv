#ifndef OPTIONS_H
#define OPTIONS_H

#define CAP_DEVICE_ID 0
#define DEV_WIDTH 320
#define TOLERANCE 1000

enum verbosity_t
{
    v_debug = 0,
    v_info = 1,
    v_warn = 2,
    v_error = 3
};

typedef unsigned int ui_t;
typedef struct cmd_options_s
{
    ui_t deviceId = CAP_DEVICE_ID;
    ui_t threshold = TOLERANCE;
    ui_t width = DEV_WIDTH;
    ui_t verbosity = v_info;
} cmd_options_t;

#endif