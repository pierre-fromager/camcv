#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <string>
#include <algorithm>

#define FMT_TIME "%Y%m%d%H%M%S"
#define LINE_FEED '\n'

namespace Tools
{
    class Timestamp
    {

    public:
        static time_t now();
        static std::string asNumber();
        static std::string asDate();
    };
}
#endif // TIMESTAMP_H