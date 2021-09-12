#ifndef YTC_RENDERABLE_BUFFER_HPP
#define YTC_RENDERABLE_BUFFER_HPP

#include "Core/Fundation.hpp"
namespace YtcGE
{
    template<typename Pixel>
    class RenderableBuffer
    {
    public:
        static constexpr float ZMin = -1.0f;
        RenderableBuffer(void * buffer, uint32_t w, uint32_t h) noexcept
            : pixel_buffer_(static_cast<Pixel*>(buffer)),
              width_(w), height_(h), size_(w * h)
        {
            z_buffer_ = std::vector<float>(size_, ZMin);
        }

        ~RenderableBuffer() noexcept
        {
        }

        static constexpr uint32_t Stride() noexcept
        {
            return sizeof(Pixel);
        }

        void SetPixel(uint32_t x, uint32_t y, Pixel value, float z = ZMin) noexcept
        {
            int pos = y * width_ + x;
            if (z >= z_buffer_[pos])
            {
                z_buffer_[pos] = z;
				        pixel_buffer_[pos] = value;
            }
        }

        void FillWith(Pixel pixel) noexcept
        {
            std::fill(pixel_buffer_, pixel_buffer_ + Size(), pixel);
            std::fill(z_buffer_.begin(), z_buffer_.end(), ZMin);
        }

        const std::vector<float>& ZBuffer() const noexcept
        {
            return z_buffer_;
        }

        constexpr uint8_t* PixelBuffer() const noexcept
        {
            return reinterpret_cast<uint8_t*>(pixel_buffer_);
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
        Pixel * pixel_buffer_;
        std::vector<float> z_buffer_;
        uint32_t size_;
        uint32_t width_;
        uint32_t height_;
    };
    
}

#endif