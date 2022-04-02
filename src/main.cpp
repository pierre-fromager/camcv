
#include <camcv/config.h>
#include <optionsparser.h>
#include <tools/timestamp.h>

/**
 * @brief init gui
 * 
 * @param cmdopts options
 */
static void initGui(cmd_options_t &cmdopts)
{
    cv::namedWindow(WINDOW_TITLE);
    cv::createTrackbar(TKB_TYPES, WINDOW_TITLE, &cmdopts.filter_type, FILTER_MAX_TYPE);
    cv::createTrackbar(TKB_VALUE, WINDOW_TITLE, &cmdopts.filter_value, FILTER_MAX_VALUE);
}

/**
 * @brief convert cap to gray
 *
 * @param m initial
 * @param mGray grayed
 */
static void toGray(const cv::Mat m, cv::Mat &mGray)
{
    cv::cvtColor(m, mGray, cv::COLOR_RGB2GRAY);
}

/**
 * @brief filter
 *
 * @param m initial
 * @param mFilter filtered
 * @param opts
 */
static void toFilter(const cv::Mat m, cv::Mat &mFilter, cmd_options_t opts)
{
    cv::threshold(m, mFilter, opts.filter_value, FILTER_MAX_VALUE, opts.filter_type);
}

/**
 * @brief compare current and previous capture
 *
 * @param m1 current img
 * @param m2 previous img
 * @param diff diff img
 * @param opts options
 * @return int non zero diff
 */
static int compare(const cv::Mat m1, const cv::Mat m2, cv::Mat &diff, cmd_options_t opts)
{
    if (m1.empty() && m2.empty())
        return -1;
    if (m1.cols != m2.cols || m1.rows != m2.rows || m1.dims != m2.dims)
        return -2;
    cv::Mat m1g, m2g;
    toGray(m1, m1g);
    toGray(m2, m2g);
    if (opts.filter_type + opts.filter_value != 0)
    {
        cv::Mat m1b, m2b;
        toFilter(m1g, m1b, opts);
        toFilter(m2g, m2b, opts);
        cv::compare(m1b, m2b, diff, cv::CMP_NE);
    }
    else
        cv::compare(m1g, m2g, diff, cv::CMP_NE);
    return cv::countNonZero(diff);
}

/**
 * @brief trigger action when threshold outmoded
 *
 * @param img current capture
 * @param opts options parser results
 */
static void action(const cv::Mat img, int deltaDiff, int frames, cmd_options_t opts)
{
    const std::string ts = Tools::Timestamp::asNumber();
    if (opts.verbosity <= v_info)
        std::cout << MOTIONAT_MSG << ts << ":" << deltaDiff << std::endl;
    if (opts.savimg)
    {
        char filename[21];
        sprintf(filename, "%s%02d%s", ts.c_str(), frames, JPEG_EXT);
        cv::imwrite(filename, img);
    }
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
    int frames = 0;
    int diffValue, diffPrev = 0;
    bool diffMode = false;
    cv::Mat img, imgPrev, imgDiff; // Matrices
    if (cmdopts.gui)               // Gui properties
        initGui(cmdopts);
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
            if (frames > MAX_FRAMES)
                frames = 0;
            capdev >> img;                                       // Capture image
            diffValue = compare(imgPrev, img, imgDiff, cmdopts); // Compare captures
            if (cmdopts.gui)                                     // Display GUI img
                cv::imshow(WINDOW_TITLE, (diffMode) ? imgDiff : img);
            const std::string tsDate = Tools::Timestamp::asDate(); // Timestamp as date
            // check@interval
            if (frames % cmdopts.cintval == 0)
            {
                const int deltaDiff = abs(diffPrev - diffValue);
                if ((ui_t)deltaDiff > cmdopts.threshold)
                    action(img, deltaDiff, frames, cmdopts);
                capdev >> imgPrev;
            }
            else
                diffPrev = diffValue;
            if (cmdopts.gui) // Keys management
            {
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
    }
    return EXIT_SUCCESS;
}