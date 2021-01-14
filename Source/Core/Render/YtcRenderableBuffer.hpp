#ifndef YTC_RENDERABLE_BUFFER_HPP
#define YTC_RENDERABLE_BUFFER_HPP

#include "Core/Fundation.hpp"
namespace YtcGE
{
    template<typename Pixel>
    class RenderableBuffer
    {
    public:
        RenderableBuffer(void * buffer, uint32_t w, uint32_t h) noexcept
            : buffer_(static_cast<Pixel*>(buffer)),
              width_(w), height_(h)
        {
        }

        ~RenderableBuffer() noexcept
        {
        }

        static constexpr uint32_t Stride() noexcept
        {
            return sizeof(Pixel);
        }

        void SetPixel(uint32_t x, uint32_t y, Pixel value) noexcept
        {
            buffer_[y * width_ + x] = value;
        }

        constexpr uint8_t* Buffer() const noexcept
        {
            return reinterpret_cast<uint8_t*>(buffer_);
        }

        constexpr uint32_t Size() const noexcept
        {
            return width_ * height_ * Stride;
        }

    private:
        Pixel * buffer_;
        uint32_t size_;
        uint32_t width_;
        uint32_t height_;
    };
    
}

#endif