
#include <device.h>

Device::Device(int deviceId) : m_deviceId(deviceId)
{
    m_device.open(deviceId);
}

Device::~Device()
{
    m_device.release();
}

device_capability_t Device::properties()
{
    m_properties.frame_width = m_device.get(cv::CAP_PROP_FRAME_WIDTH);
    m_properties.frame_height = m_device.get(cv::CAP_PROP_FRAME_HEIGHT);
    m_properties.frame_fps = m_device.get(cv::CAP_PROP_FPS);
    m_properties.pos_msec = m_device.get(cv::CAP_PROP_POS_MSEC);
    m_properties.frame_count = m_device.get(cv::CAP_PROP_FRAME_COUNT);
    m_properties.brightness = m_device.get(cv::CAP_PROP_BRIGHTNESS);
    m_properties.contrast = m_device.get(cv::CAP_PROP_CONTRAST);
    m_properties.saturation = m_device.get(cv::CAP_PROP_SATURATION);
    m_properties.hue = m_device.get(cv::CAP_PROP_HUE);
    m_properties.gain = m_device.get(cv::CAP_PROP_GAIN);
    m_properties.convert_rgb = m_device.get(cv::CAP_PROP_CONVERT_RGB);
    return m_properties;
}

bool Device::setProp(int propId, double val)
{
    // @TODO : Below devive specific props to implement
    // capdev.set(cv::CAP_PROP_FPS, CAP_FPS);
    // capdev.set(cv::CAP_PROP_FOURCC, CAP_CODEC);
    // capdev.set(cv::CAP_PROP_EXPOSURE, CAP_EXPOSURE);
    return m_device.set(propId, val);
}

bool Device::ready()
{
    return m_device.isOpened();
}

void Device::capture(cv::Mat &img)
{
    m_device >> img;
}