
#include <logger.h>

Logger::Logger(std::string path, unsigned int level) : m_level(level)
{
    m_file.open(path);
}

Logger::~Logger()
{
    m_file.close();
}

void Logger::logDebug(std::string msg)
{
    log(msg, v_debug);
}

void Logger::logInfo(std::string msg)
{
    log(msg, v_info);
}

void Logger::logWarn(std::string msg)
{
    log(msg, v_warn);
}

void Logger::logErr(std::string msg)
{
    log(msg, v_error);
}

void Logger::log(std::string msg, unsigned int verbosity)
{
    if (verbosity >= m_level)
        m_file << getDT() << " " << vlabel.at(verbosity) << " " << msg << std::endl;
}

std::string Logger::getDT()
{
    const auto t = std::time(nullptr);
    const auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, LOGGER_DT_FMT);
    return oss.str();
}