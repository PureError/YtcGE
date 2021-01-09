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
        ifstream inf(filepath);
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
            TEMP_LOG("bmp header size:%d, file size:%d, data offset:%d,"
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
            i.stride = ((i.width * bytes_count) + 3) & ~3;
            auto size = i.stride * i.width;
            i.data.resize(size);
            const auto len_row = i.width * bytes_count;
            std::vector<char> row(i.width * len_row);
            inf.seekg(bfh.off_bits, std::ios::beg);
            for (uint32_t y = 0; y < i.height; ++y)
            {
                inf.read(row.data(), len_row);
                for (uint32_t x = 0; x < i.width; ++x)
                {
                    auto diff = y * i.width + x;
                    auto src = *reinterpret_cast<uint8_t*>(row.data() + diff);
                    auto dst = reinterpret_cast<uint8_t*>(i.data.data() + diff);
                    uint32_t pixel = 0xFF;
                    memcpy(&pixel, &src, bytes_count);
                    *dst = ((pixel & 0x0000FF00) << 16) | (pixel & 0x00FF0000) | ((pixel & 0xFF000000) >> 16);
                }
            }
            fmt = Format::BMP;
        }
        else
        {
            throw Exception(_T("Sorry, no implemention for this img format!"));
        }
        return img;
    }

}