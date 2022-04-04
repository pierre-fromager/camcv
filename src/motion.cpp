
#include <motion.h>

Motion::Motion()
{
}

Motion::~Motion()
{
}

void Motion::setOptions(cmd_options_t options)
{
    m_options = options;
}

void Motion::setFrame(cv::Mat frame)
{
    m_current = frame;
}

void Motion::setFramePrevious()
{
    m_previous = m_current.clone();
}

int Motion::detect()
{
    FrameCompare();
    return cv::countNonZero(m_diff);
}

cv::Mat Motion::getCurrentFrame()
{
    return m_current;
}

cv::Mat Motion::getDiffFrame()
{
    return m_diff;
}

void Motion::FrameToGray(const cv::Mat m, cv::Mat &mGray)
{
    cv::cvtColor(m, mGray, cv::COLOR_RGB2GRAY);
}

void Motion::FrameToFilter(const cv::Mat m, cv::Mat &mFilter)
{
    cv::threshold(m, mFilter, m_options.filter_value, FILTER_MAX_VALUE, m_options.filter_type);
}

void Motion::FrameCompare()
{
    if (CompareReady())
    {
        cv::Mat m1g, m2g;
        FrameToGray(m_current, m1g);
        FrameToGray(m_previous, m2g);
        if (m_options.filter_type + m_options.filter_value != 0)
        {
            cv::Mat m1b, m2b;
            FrameToFilter(m1g, m1b);
            FrameToFilter(m2g, m2b);
            cv::compare(m1b, m2b, m_diff, cv::CMP_NE);
        }
        else
            cv::compare(m1g, m2g, m_diff, cv::CMP_NE);
    }
}

bool Motion::CompareReady()
{
    if (m_current.empty() || m_previous.empty())
        return false;
    if (m_current.cols != m_previous.cols || m_current.rows != m_previous.rows || m_current.dims != m_previous.dims)
        return false;
    return true;
}