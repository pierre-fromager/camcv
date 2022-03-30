
#include <camcv/config.h>

/**
 * @brief timestamp
 *
 * @return std::string
 */
std::string timestamp()
{
    time_t now = time(0);
    char *dt = ctime(&now);
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    std::string ts(dt);
    ts.erase(std::remove(ts.begin(), ts.end(), LINE_FEED), ts.end());
    return ts;
}

/**
 * @brief compare 2 images and returns non zero diff amount
 *
 * @param m1 current captured image
 * @param m2 previous captured image
 * @return int
 */
int diffCap(const cv::Mat m1, const cv::Mat m2)
{
    cv::Mat m1g, m2g, diff;
    if (m1.empty() && m2.empty())
        return -1;
    if (m1.cols != m2.cols || m1.rows != m2.rows || m1.dims != m2.dims)
        return -2;
    // Converting captures to gray
    cv::cvtColor(m1, m1g, cv::COLOR_RGB2GRAY);
    cv::cvtColor(m2, m2g, cv::COLOR_RGB2GRAY);
    // Comparing
    cv::compare(m1g, m2g, diff, cv::CMP_NE);
    // Amount of diffs
    return cv::countNonZero(diff);
}

int main()
{
    std::cout << MAIN_MSG << std::endl;
    unsigned long int frames = 0;
    int diffValue = 0;
    // Matrix
    cv::Mat img, imgPrev;
    // Window named
    cv::namedWindow(WINDOW_TITLE);
    // Set capture device
    cv::VideoCapture capdev(CAP_DEVICE_ID);
    // Set device width
    capdev.set(cv::CAP_PROP_FRAME_WIDTH, DEV_WIDTH);
    // Error if cannot open device
    if (!capdev.isOpened())
    {
        std::cout << ERR_MSG << std::endl;
        exit(EXIT_FAILURE);
    }
    else
    {
        capdev >> imgPrev;
        while (capdev.isOpened())
        {
            frames++;
            // Copy capture image to matrix
            capdev >> img;
            // Display matrix on window frame
            cv::imshow(WINDOW_TITLE, img);
            diffValue = diffCap(imgPrev, img);
            // Console out differences captures
            if (DEBUG)
                std::cout << timestamp() << SPACE << frames << SPACE << DIFF_WEIGHT_LABEL << diffValue << std::endl;
            if (frames % POLLING_CAP_INTERVAL == 0)
            {
                if (DEBUG)
                    std::cout << timestamp() << SPACE << frames << SPACE << CAPTURE_LABEL << std::endl;
                capdev >> imgPrev;
            }
            // Exit if ESC keypressed
            if (cv::waitKey(POLLING_KEY_TIME) == ESC_CODE)
                break;
        }
    }
    return EXIT_SUCCESS;
}