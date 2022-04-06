
#include <camcv/config.h>
#include <optionsparser.h>
#include <tools/timestamp.h>
#include <logger.h>
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
 * @param img current capture filename
 * @param deltaDiff motion weight
 * @param frames frame number
 * @param opts options
 * @param logger
 */
static void action(const cv::Mat img, const int deltaDiff, const int frames, const cmd_options_t opts, Logger *logger)
{
    const std::string ts = Tools::Timestamp::asNumber();
    std::ostringstream actionMsg;
    actionMsg << MOTIONAT_MSG << ts << "@" << frames << ":" << deltaDiff;
    logger->logInfo(actionMsg.str());
    if (opts.savimg)
    {
        char tsframe[ACTION_TSFRAME_SIZE];
        const std::string ext = JPEG_EXT;
        sprintf(tsframe, "%s%02d", ts.c_str(), frames);
        cv::putText(img, tsframe, cv::Point(10, 10), cv::FONT_HERSHEY_PLAIN, 0.5, cv::Scalar(0, 0, 255), 1);
        cv::imwrite(tsframe + ext, img);
    }
}

static int parseOptions(int argc, char **argv, cmd_options_t &cmdopts)
{
    ui_t rcopt;
    try
    {
        auto *opParser = new OptionsParser(argc, argv);
        rcopt = opParser->parse(cmdopts);
        if (cmdopts.verbosity == v_debug)
            opParser->debug(cmdopts);
        delete opParser;
    }
    catch (const bpo::error &ex)
    {
        std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
    }
    return rcopt;
}

static void checkFolders(cmd_options_t opts, Logger *logger)
{
    const char *path = opts.cappath.c_str();
    namespace fs = boost::filesystem;
    fs::path dir(path);
    if (fs::create_directory(dir))
        logger->logDebug(MSG_CAP_FOLDER_SUCCESS + opts.cappath);
    else
        logger->logErr(MSG_CAP_FOLDER_FAIL + opts.cappath);
}

int main(int argc, char **argv)
{
    cmd_options_t cmdopts;
    if (parseOptions(argc, argv, cmdopts) == EXIT_FAILURE)
        return EXIT_FAILURE;
    auto logger = new Logger(LOGGER_PATH, cmdopts.verbosity);
    checkFolders(cmdopts, logger);
    int frames, diffValue, diffPrev = 0;
    bool diffMode = false;
    cv::Mat img;     // Captured image
    if (cmdopts.gui) // Gui
        initGui(cmdopts);
    auto *dev = new Device(cmdopts.deviceId, logger);
    dev->setProp(cv::CAP_PROP_FRAME_WIDTH, cmdopts.width);
    if (false == dev->ready())
    {
        logger->logErr(ERR_MSG);
        return EXIT_FAILURE;
    }
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
            cv::imshow(WINDOW_TITLE, (diffMode) ? motion->getDiffFrame()
                                                : motion->getCurrentFrame());
        if (frames % cmdopts.cintval == 0) // check diff
        {
            motion->setFramePrevious();
            const int deltaDiff = abs(diffPrev - diffValue);
            if (deltaDiff > cmdopts.threshold)
                action(img, deltaDiff, frames, cmdopts, logger);
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
    delete dev;
    delete logger;
    return EXIT_SUCCESS;
}