#include "YtcTexture.hpp"
#include <fstream>

using namespace std;

namespace YtcGE
{

    YtcGE::Texture2D::~Texture2D() noexcept
    {

    }

    Texture2D::Texture2D(Image::Ptr image) noexcept: img_(std::move(image))
    {

    }

    YtcGE::Color32b Texture2D::Sample(float u, float v) const
    {
        Color32b c(0, 0, 0, 0);
        uint32_t max_x = Width() - 1;
        uint32_t max_y = Height() - 1;
        auto x = static_cast<uint32_t>(u * max_x + 0.5f);
        auto y = static_cast<uint32_t>(u * max_y + 0.5f);
        x = Clamp(x, 0u, max_x);
        y = Clamp(y, 0u, max_y);
        const auto & img = *img_;
        const auto * bits = &(img.data[(y * Width() + x) * 4]);
        //const auto stride = Min((img.stride >> 3), c.Size());
        memcpy(&c, bits, c.Size());
        return c;
    }

}

