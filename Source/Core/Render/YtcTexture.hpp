#ifndef YTC_TEXTURE_HPP
#define YTC_TEXTURE_HPP

#include "YtcColor.hpp"
#include "YtcImage.hpp"
#include <vector>
#include <cstdint>
#include <memory>
namespace YtcGE
{
    class Texture2D : public std::enable_shared_from_this<Texture2D>
    {
    public:
        using Ptr = std::shared_ptr<Texture2D>;
        Texture2D(Image::Ptr image) noexcept;
        ~Texture2D() noexcept;

        uint32_t Width() const noexcept
        {
            return img_->width;
        }

        uint32_t Height() const noexcept
        {
            return img_->height;
        }

        Color32b Sample(float u, float v) const;
    private:
        Image::Ptr img_;
    };
}
#endif
