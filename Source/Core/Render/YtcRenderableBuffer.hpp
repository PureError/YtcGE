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
              width_(w), height_(h), size_(w * h)
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

        void FillWith(Pixel pixel) noexcept
        {
            std::fill(buffer_, buffer_ + Size(), pixel);
        }

        constexpr uint8_t* Buffer() const noexcept
        {
            return reinterpret_cast<uint8_t*>(buffer_);
        }

        constexpr uint32_t SizeInBytes() const noexcept
        {
            return width_ * height_ * Stride();
        }

        constexpr uint32_t Size() const noexcept
        {
            return size_;
        }

        constexpr uint32_t Width() const noexcept
        {
            return width_;
        }

        constexpr uint32_t Height() const noexcept
        {
            return height_;
        }

    private:
        Pixel * buffer_;
        uint32_t size_;
        uint32_t width_;
        uint32_t height_;
    };
    
}

#endif