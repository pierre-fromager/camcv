
#include <opencv4/opencv2/opencv.hpp>

#define WINDOW_TITLE "Display window"
#define ERR_MSG "Cannot open camera"
#define DEVICE_ID 0
#define POLLING_TIME 25
#define ESC_CODE 27
#define DEV_WIDTH 320

int diffAmount(const cv::Mat m1, const cv::Mat m2)
{
    const int CV_RGB2GRAY = 7;
    cv::Mat m1g, m2g, diff;
    cv::cvtColor(m1, m1g, CV_RGB2GRAY, 1);
    cv::cvtColor(m2, m2g, CV_RGB2GRAY, 1);
    if (m1.empty() && m2.empty())
        return -1;
    if (m1.cols != m2.cols || m1.rows != m2.rows || m1.dims != m2.dims)
        return -2;
    cv::compare(m1g, m2g, diff, cv::CMP_NE);
    return cv::countNonZero(diff);
}

int main()
{
    unsigned long int frames = 0;
    // Matrix
    cv::Mat img, imgPrev;
    // Window named
    cv::namedWindow(WINDOW_TITLE);
    // Set capture device
    cv::VideoCapture capdev(DEVICE_ID);
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
        while (true)
        {
            frames++;
            // Copy capture image to matrix
            capdev >> img;
            // Display matrix on window frame
            cv::imshow(WINDOW_TITLE, img);
            // Console differences
            std::cout << "Differences = " << diffAmount(imgPrev, img) << std::endl;
            // Exit if ESC keypressed
            if (cv::waitKey(POLLING_TIME) == ESC_CODE)
                break;
            if (frames == POLLING_TIME)
            {
                //imgPrev = img;
                capdev >> imgPrev;
                frames = 0;
            }
        }
    }
    return EXIT_SUCCESS;
}