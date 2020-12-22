#pragma once
#ifndef YTC_LOG_HPP
#define YTC_LOG_HPP


#include "Core/StringUtils/YtcString.hpp"
#include <stdarg.h>

namespace YtcGE
{
    inline void PrintLog(const AString& fmt, const char* srcFile, const char* func, int line, ...)
    {
        std::stringstream ss;
        ss << "File:" << srcFile << "| Function:" << func << "| Line:" << line << "| Message:" << fmt;
        va_list vlist;
        va_start(vlist, line);
        vprintf(ss.str().c_str(), vlist);
        va_end(vlist);
    }

}

#define TEMP_LOG(fmt, ...) ::YtcGE::PrintLog(fmt, __FILE__, __FUNCTION__, __LINE__, ##__VA_ARGS__)
#endif
