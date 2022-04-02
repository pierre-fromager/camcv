
#include <tools/timestamp.h>

time_t Tools::Timestamp::now()
{
    return time(nullptr);
}

std::string Tools::Timestamp::asNumber()
{
    const time_t now = Tools::Timestamp::now();
    std::string ts = "";
    struct tm *lt = localtime(&now);
    char tstr[15];
    struct tm *pTime = localtime(&now);
    strftime(tstr, 15, FMT_TIME, pTime);
    ts += tstr;
    return ts;
}

std::string Tools::Timestamp::asDate()
{
    const time_t now = Tools::Timestamp::now();
    std::string ts = "";
    char *dt = ctime(&now);
    tm *gmtm = gmtime(&now);
    dt = asctime(gmtm);
    ts += dt;
    ts.erase(std::remove(ts.begin(), ts.end(), LINE_FEED), ts.end());
    return ts;
}
