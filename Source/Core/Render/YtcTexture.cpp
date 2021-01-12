#include "YtcTexture.hpp"
#include <fstream>

using namespace std;

namespace YtcGE
{

    YtcGE::Texture2D::~Texture2D()
    {

    }

    Texture2D::Texture2D(Image::Ptr image) : img_(std::move(image))
    {

    }

    YtcGE::Color32b Texture2D::Sample(float u, float v) const
    {
        Color32b c(0, 0, 0, 0);
        auto x = static_cast<uint32_t>(u * (Width() - 1) + 0.5f);
        auto y = static_cast<uint32_t>(u * (Height() - 1)+ 0.5f);
        const auto & img = *img_;
        const auto * bits = &(img.data[(y * Width() + x) * 4]);
        //const auto stride = Min((img.stride >> 3), c.Size());
        memcpy(&c, bits, c.Size());
        return c;
    }

}

