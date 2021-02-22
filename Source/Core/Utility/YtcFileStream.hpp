#ifndef YTC_FILE_STREAM_HPP
#define YTC_FILE_STREAM_HPP
#include "Core/Fundation.hpp"
#include <fstream>


namespace YtcGE
{
#ifdef YTC_UNICODE
    using IFileStream = std::wifstream;
    using OFileStream = std::wofstream;
    using FileStream = std::wfstream;
#else
    using IFileStream = std::ifstream;
    using OFileStream = std::ofstream;
    using FileStream = std::fstream;
#endif
}

#endif