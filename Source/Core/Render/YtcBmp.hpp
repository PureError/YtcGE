#ifndef YTC_BMP_HPP
#define YTC_BMP_HPP

#include <cstdint>
namespace YtcGE
{
#pragma pack(push, 1)
    struct BitmapFileiHeader
    {
        uint16_t type; //The file type; must be BM.
        uint32_t size; //The size, in bytes, of the bitmap file.
        uint16_t reserved[2];//Reserved; must be zero.
        uint32_t off_bits; //The offset, in bytes, from the beginning of the structure to the bitmap bits.
    };

    struct CIEXYZ
    {
        int32_t x;
        int32_t y;
        int32_t z;
    };

    struct CIEXYZTriple
    {
        CIEXYZ red;
        CIEXYZ green;
        CIEXYZ blue;
    };

    struct BitmapInfoHeader
    {
        uint32_t size; //The number of bytes required by the structure.
        int32_t width; //The width of the bitmap, in pixels.
        int32_t height; //The height of the bitmap, in pixels.
        uint16_t planes; //The number of planes for the target device. This value must be set to 1.
        uint16_t bit_count;//The number of bits-per-pixel.
        uint32_t compression; //The type of compression for a compressed bottom-up bitmap (top-down DIBs cannot be compressed). This member can be one of the following values.
        uint32_t size_image; //The size, in bytes, of the image. This may be set to zero for BI_RGB bitmaps.
        int32_t x_pels_per_meter; //The horizontal resolution, in pixels-per-meter, of the target device for the bitmap.
        int32_t y_pels_per_meter; //The vertical resolution, in pixels-per-meter, of the target device for the bitmap.
        uint32_t clr_used; //The number of color indexes in the color table that are actually used by the bitmap.
        uint32_t clr_important; //The number of color indexes that are required for displaying the bitmap. If this value is zero, all colors are required.
        uint32_t red_mask; //Color mask that specifies the red component of each pixel
        uint32_t green_mask; //Color mask that specifies the green component of each pixel
        uint32_t blue_mask; //Color mask that specifies the blue component of each pixel
        uint32_t alpha_mask; //Color mask that specifies the alpha component of each pixel.
        uint32_t clr_space_type; //The color space
        CIEXYZTriple endpoints; //Correspond to the red, green, and blue endpoints for the logical color space associated with the bitmap
        uint32_t gamma_red; //Toned response curve for red
        uint32_t gamma_green; //Toned response curve for green
        uint32_t gamma_blue; //Toned response curve for blue
        uint32_t intent; //Rendering intent for bitmap.
        uint32_t profile_data; //The offset, in bytes, from the beginning of the structure to the start of the profile data. 
        uint32_t profile_size; //Size, in bytes, of embedded profile data.
        uint32_t reserved; //This member has been reserved. Its value should be set to zero.
    };



    struct RGBQuard
    {
        uint8_t b;
        uint8_t g;
        uint8_t r;
        uint8_t reserved;
    };

#pragma pack(pop)
}

#endif