#include "YtcImage.hpp"
#include "YtcBmp.hpp"
#include "../Utility/YtcException.hpp"
#include "../Log/YtcLog.hpp"
#include <fstream>
using namespace std;
namespace YtcGE
{

    Image::Ptr Image::FromFile(const String & filepath)
    {
        Image::Format fmt;
        ifstream inf(filepath, std::ios::binary);
        inf.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        if (!inf.is_open())
        {
            throw Exception(_T("Failed to open file :") + filepath);
        }
        Image::Ptr img = std::make_shared<Image>();
        Image & i = *img;
        if (filepath.EndWith(_T(".bmp")))
        {
            BitmapFileiHeader bfh;
            inf.read((char*)&bfh, sizeof(bfh));
            if (bfh.type != 0x4D42)
            {
                throw Exception(_T("Unrecognized file type!"));
            }
            uint32_t size_info_header = 0;
            inf.read((char*)&size_info_header, sizeof(size_info_header));
            BitmapInfoHeader bih = {};
            bih.size = size_info_header;
            inf.read((char*)(&bih) + (sizeof(bih.size)), size_info_header - sizeof(bih.size));
            YTC_LOG("bmp header size:%d, file size:%d, data offset:%d,"
                     "width:%d, height:%d, planes:%d£¬bitcount:%d, compression:%d, imgage size:%d",
                     size_info_header, bfh.size, bfh.off_bits, bih.width, bih.height, bih.planes,
                     bih.bit_count, bih.compression, bih.size_image);
            if (bih.bit_count != 32 && bih.bit_count != 24)
            {
                throw Exception(_T("Only 32 or 24 bit image is supported!"));
            }

            if (bih.height < 0)
            {
                throw Exception(_T("Negatvie height is not supperted!"));
            }
            i.width = bih.width;
            i.height = bih.height;
            i.bit_count = bih.bit_count;
            i.compression = bih.compression;
            auto bytes_count = i.bit_count >> 3;
            auto stride = ((i.width * bytes_count) + 3) & ~3;
            auto size = 4 * i.width * i.height; //i.stride * i.width;
            i.data.resize(size);
            std::vector<char> row(stride);
            inf.seekg(bfh.off_bits, std::ios::beg);
            for (uint32_t y = 0; y < i.height; ++y)
            {
                inf.read(row.data(), stride);
                auto dst_row_begin = y * i.width * 4;
                for (uint32_t x = 0; x < i.width; ++x)
                {
                    auto diff_dst = dst_row_begin + x * 4;
                    auto src = reinterpret_cast<uint8_t*>(row.data() + x * bytes_count);
                    auto dst = reinterpret_cast<uint8_t*>(i.data.data() + diff_dst);
                    uint32_t pixel = 0xFF000000;
                    memcpy(&pixel, src, bytes_count);
                    pixel = ((pixel & 0x000000FF) << 16) | (pixel & 0x0000FF00) | ((pixel & 0x00FF0000) >> 16) | ((pixel & 0xFF000000));
                    *reinterpret_cast<uint32_t*>(dst) = pixel;
                }
            }
            fmt = Format::BMP;
        }
        else
        {
            throw Exception(_T("Sorry, no implemention for this image-format!"));
        }
        return img;
    }

}