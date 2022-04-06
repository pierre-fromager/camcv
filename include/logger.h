#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <map>
#include <verbosity.h>

#define LOGGER_DT_FMT "%Y-%m-%d-%H:%M:%S"

class Logger
{
public:
    Logger(std::string filepath, unsigned int level);
    ~Logger();
    void logDebug(std::string msg);
    void logInfo(std::string msg);
    void logWarn(std::string msg);
    void logErr(std::string msg);

private:
    std::ofstream m_file;
    unsigned int m_level;
    const std::map<unsigned int, std::string> vlabel{
        {v_debug, "DEBUG"},
        {v_info, "INFO"},
        {v_warn, "WARN"},
        {v_error, "ERR"},
    };
    void log(std::string msg, unsigned int verbosity);
    std::string getDT();
};

#endif // LOGGER_H