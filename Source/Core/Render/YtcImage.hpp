#ifndef YTC_IMAGE_HPP
#define YTC_IMAGE_HPP

#include "../StringUtils/YtcString.hpp"
#include <cstdint>
#include <vector>
#include <fstream>
#include <memory>
namespace YtcGE
{
  
    struct Image
    {
        enum class Format
        {
            BMP,
            PNG,
            JPEG,
        };
        using Ptr = std::shared_ptr<Image>;
        static Ptr FromFile(const String & filepath);
        uint32_t width;
        uint32_t height;
        //uint32_t stride;
        uint32_t bit_count;
        uint32_t compression;
        Format format;
        std::vector<uint8_t> data;
    };
}

#endif