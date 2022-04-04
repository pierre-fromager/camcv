#ifndef DEVICE_H
#define DEVICE_H
#include <camcv/config.h>

typedef struct device_capability_s
{
    double frame_width;
    double frame_height;
    double frame_fps;
    double pos_msec;
    double frame_count;
    double brightness;
    double contrast;
    double saturation;
    double hue;
    double gain;
    double convert_rgb;
} device_capability_t;

class Device
{

public:
    explicit Device(int deviceId);
    ~Device();
    device_capability_t properties();
    bool setProp(int propId, double val);
    bool ready();
    void capture(cv::Mat &m_img);

private:
    cv::VideoCapture m_device;
    int m_deviceId;
    device_capability_t m_properties;
};

#endif // DEVICE_H