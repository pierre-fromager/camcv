#ifndef MOTION_H
#define MOTION_H

#include <camcv/config.h>
#include <options.h>

class Motion
{

public:
    explicit Motion();
    ~Motion();
    /**
     * @brief Set the Options object
     *
     * @param options
     */
    void setOptions(cmd_options_t options);
    /**
     * @brief Set the current frame object
     * 
     * @param frame 
     */
    void setFrame(cv::Mat frame);
    /**
     * @brief Set previous frame from current
     * 
     */
    void setFramePrevious();

    int detect();

    cv::Mat getCurrentFrame();
    cv::Mat getDiffFrame();

private:
    /**
     * @brief options
     *
     */
    cmd_options_t m_options;
    /**
     * @brief frames
     *
     */
    cv::Mat m_current, m_previous, m_diff;
    /**
     * @brief convert frame to gray
     *
     * @param m initial
     * @param mGray grayed
     */
    void FrameToGray(const cv::Mat m, cv::Mat &mGray);
    /**
     * @brief convert frame to threshold filter
     *
     * @param m initial
     * @param mFilter filtered
     */
    void FrameToFilter(const cv::Mat m, cv::Mat &mFilter);
    /**
     * @brief compare frames and return non zero diff
     *
     * @param m1 img img
     * @param m2 previous img
     * @param diff diff img
     * @return int
     */
    void FrameCompare();
    /**
     * @brief returns true if compare matrices ok
     * 
     * @return true 
     * @return false 
     */
    bool CompareReady();
};

#endif // MOTION_H