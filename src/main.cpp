
#include <camcv/config.h>
#include <optionsparser.h>
#include <tools/timestamp.h>

/**
 * @brief mouse handler
 *
 */
static void onMouse(int event, int x, int y, int /*flags*/, void * /*param*/)
{
    if (event == cv::EVENT_LBUTTONDOWN)
        std::cout << MOUSE_EVENT << "(" << x << "," << y << ")" << std::endl;
}

/**
 * @brief convert cap to gray
 *
 * @param m
 * @param mGray
 */
static void toGray(const cv::Mat m, cv::Mat &mGray)
{
    cv::cvtColor(m, mGray, cv::COLOR_RGB2GRAY);
}

/**
 * @brief compare current and previous capture
 *
 * @param m1 current img
 * @param m2 previous img
 * @param diff diff img
 * @return int non zero diff
 */
static int compare(const cv::Mat m1, const cv::Mat m2, cv::Mat &diff)
{
    if (m1.empty() && m2.empty())
        return -1;
    if (m1.cols != m2.cols || m1.rows != m2.rows || m1.dims != m2.dims)
        return -2;
    cv::Mat m1g, m2g;
    toGray(m1, m1g);
    toGray(m2, m2g);
    cv::compare(m1g, m2g, diff, cv::CMP_NE);
    return cv::countNonZero(diff);
}

/**
 * @brief trigger action when threshold outmoded
 *
 * @param img current capture
 * @param opts options parser results
 */
static void action(const cv::Mat img, cmd_options_t opts)
{
    const std::string ts = Tools::Timestamp::asNumber();
    if (opts.verbosity == v_debug)
        std::cout << MOTIONAT_MSG << ts << std::endl;
    if (opts.savimg)
        cv::imwrite(ts + JPEG_EXT, img);
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
            op->debug(cmdopts);
        delete op;
    }
    catch (const bpo::error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    if (rcopt == EXIT_FAILURE)
        return EXIT_FAILURE;
    unsigned long int frames = 0;
    int diffValue, diffPrev = 0;
    bool diffMode = false;
    cv::Mat img, imgPrev, imgDiff; // Matrices
    // Window properties
    cv::namedWindow(WINDOW_TITLE);
    cv::setMouseCallback(WINDOW_TITLE, onMouse, 0);
    // Capture properties
    cv::VideoCapture capdev(cmdopts.deviceId);
    capdev.set(cv::CAP_PROP_FRAME_WIDTH, cmdopts.width);
    if (false == capdev.isOpened())
    {
        std::cout << ERR_MSG << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        capdev >> imgPrev;
        while (capdev.isOpened())
        {
            frames++;
            capdev >> img;                                         // Capture image
            diffValue = compare(imgPrev, img, imgDiff);            // Compare captures
            cv::imshow(WINDOW_TITLE, (diffMode) ? imgDiff : img);  // Display UI
            const std::string tsDate = Tools::Timestamp::asDate(); // Timestamp as date
            // Debug compare
            if (cmdopts.verbosity == v_debug)
                std::cout << tsDate << SPACE << frames << SPACE << DIFF_WEIGHT_LABEL << diffValue << std::endl;
            // check@interval
            if (frames % cmdopts.cintval == 0)
            {
                if (abs(diffPrev - diffValue) > cmdopts.threshold)
                    action(img, cmdopts);
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