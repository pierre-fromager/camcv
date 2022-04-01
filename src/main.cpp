
#include <camcv/config.h>
#include <optionsparser.h>
#include <tools/timestamp.h>

/**
 * @brief returns timestamp as date or number
 *
 * @return std::string
 */
static std::string timestamp(bool asNumber = false)
{
    return (asNumber) ? Tools::Timestamp::asNumber() : Tools::Timestamp::asDate();
}

static void onMouse(int event, int x, int y, int /*flags*/, void * /*param*/)
{
    if (event == cv::EVENT_LBUTTONDOWN)
        std::cout << "Mouse clicked (" << x << "," << y << ")" << std::endl;
}

/**
 * @brief compare diff between current and previous img
 *
 * @param m1 current img
 * @param m2 previous img
 * @param diff diff img
 * @return int
 */
static int compare(const cv::Mat m1, const cv::Mat m2, cv::Mat &diff)
{
    if (m1.empty() && m2.empty())
        return -1;
    if (m1.cols != m2.cols || m1.rows != m2.rows || m1.dims != m2.dims)
        return -2;
    cv::Mat m1g, m2g;
    // Converting captures to gray
    cv::cvtColor(m1, m1g, cv::COLOR_RGB2GRAY);
    cv::cvtColor(m2, m2g, cv::COLOR_RGB2GRAY);
    // Comparing
    cv::compare(m1g, m2g, diff, cv::CMP_NE);
    // Amount of diffs
    return cv::countNonZero(diff);
}

/**
 * @brief action triggered when threshold overhead
 *
 * @param msg
 * @param qntDiff
 */
static void action(const cv::Mat img, std::string filename, std::string msg, int qntDiff)
{
    std::cout << msg << qntDiff << std::endl;
    cv::imwrite(filename, img);
}

int main(int argc, char **argv)
{
    cmd_options_t cmdopts;
    ui_t rcopt;
    try
    {
        auto *op = new OptionsParser(argc, argv);
        rcopt = op->parse(cmdopts);
        if (cmdopts.verbosity == v_debug)
            std::cout << MAIN_MSG
                      << " DeviceId:" << cmdopts.deviceId
                      << " Threshold:" << cmdopts.threshold
                      << " Width:" << cmdopts.width
                      << " Verbosity:" << cmdopts.verbosity
                      << std::endl;
        delete op;
    }
    catch (const bpo::error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    if (rcopt == EXIT_FAILURE)
        return 1;

    unsigned long int frames = 0;
    int diffValue, diffPrev = 0;
    bool diffMode = false;
    // Matrix
    cv::Mat img, imgPrev, imgDiff;
    // Window named
    cv::namedWindow(WINDOW_TITLE);
    cv::setMouseCallback(WINDOW_TITLE, onMouse, 0);
    // Set capture device
    cv::VideoCapture capdev(cmdopts.deviceId);
    // Set device width
    capdev.set(cv::CAP_PROP_FRAME_WIDTH, cmdopts.width);
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
            // compare captures
            diffValue = compare(imgPrev, img, imgDiff);
            // Display matrix on window frame
            cv::imshow(WINDOW_TITLE, (diffMode) ? imgDiff : img);
            // Console out differences captures
            const std::string ts = timestamp();
            if (cmdopts.verbosity == v_debug)
                std::cout << ts << SPACE << frames << SPACE << DIFF_WEIGHT_LABEL << diffValue << std::endl;
            // Check diff @interval
            if (frames % POLLING_CAP_INTERVAL == 0)
            {
                if (cmdopts.verbosity == v_debug)
                    std::cout << ts << SPACE << frames << SPACE << CAPTURE_LABEL << std::endl;
                const ui_t qntDiff = abs(diffPrev - diffValue);
                if (qntDiff > cmdopts.threshold)
                    action(img, timestamp(true) + JPEG_EXT, ts + SMOTION_DETECTED_LABEL, qntDiff);
                capdev >> imgPrev;
            }
            else
                diffPrev = diffValue;
            // Keys management
            const int kv = cv::waitKey(POLLING_KEY_TIME);
            if (kv == ESC_CODE)
                break;
            switch (kv)
            {
            case KEY_MODE:
                diffMode = !diffMode;
                break;
            }
        }
    }
    return EXIT_SUCCESS;
}