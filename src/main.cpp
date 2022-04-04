
#include <camcv/config.h>
#include <optionsparser.h>
#include <tools/timestamp.h>
#include <device.h>
#include <motion.h>

/**
 * @brief init gui
 *
 * @param cmdopts options
 */
static void initGui(cmd_options_t &cmdopts)
{
    cv::namedWindow(WINDOW_TITLE);
    cv::createTrackbar(CTRL_FILTER_TYPE, WINDOW_TITLE, &cmdopts.filter_type, FILTER_MAX_TYPE);
    cv::createTrackbar(CTRL_FILTER_VALUE, WINDOW_TITLE, &cmdopts.filter_value, FILTER_MAX_VALUE);
    cv::createTrackbar(CTRL_TRESHOLD, WINDOW_TITLE, &cmdopts.threshold, TRESHOLD_MAX_VALUE);
}

/**
 * @brief trigger action when threshold outmoded
 *
 * @param img current capture
 * @param opts options parser results
 */
static void action(const cv::Mat img, int deltaDiff, int frames, const cmd_options_t opts)
{
    const std::string ts = Tools::Timestamp::asNumber();
    if (opts.verbosity <= v_info)
        std::cout << MOTIONAT_MSG << ts << "@" << frames << ":" << deltaDiff << std::endl;
    if (opts.savimg)
    {
        char filename[21];
        sprintf(filename, "%s%02d%s", ts.c_str(), frames, JPEG_EXT);
        cv::putText(img, ts, cv::Point(10, 10), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 255, 0), 2);
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
    cv::Mat img;     // Captured image
    if (cmdopts.gui) // Gui
        initGui(cmdopts);
    auto *dev = new Device(cmdopts.deviceId);
    dev->setProp(cv::CAP_PROP_FRAME_WIDTH, cmdopts.width);
    if (false == dev->ready())
    {
        std::cout << ERR_MSG << std::endl;
        return EXIT_FAILURE;
    }
    else
    {
        auto motion = new Motion();
        dev->capture(img);
        motion->setFrame(img);
        motion->setFramePrevious();
        while (dev->ready())
        {
            frames++;
            if (frames > MAX_FRAMES)
                frames = 0;
            dev->capture(img);
            motion->setOptions(cmdopts);  // Capture
            diffValue = motion->detect(); // Compare
            if (cmdopts.gui)              // Display GUI img
                cv::imshow(WINDOW_TITLE, (diffMode) ? motion->getDiffFrame() : img);
            if (frames % cmdopts.cintval == 0) // check diff
            {
                motion->setFramePrevious();
                const int deltaDiff = abs(diffPrev - diffValue);
                if (deltaDiff > cmdopts.threshold)
                    action(img, deltaDiff, frames, cmdopts);
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
        delete motion;
    }
    delete dev;
    return EXIT_SUCCESS;
}