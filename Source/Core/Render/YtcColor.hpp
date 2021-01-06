#ifndef YTC_COLOR_HPP
#define YTC_COLOR_HPP

#include "../Math/YtcVector.hpp"
namespace YtcGE
{
    template<typename T>
    class Color
    {
        using DetailType = Vector<T, 4>;
    public:
        using Color_T = Color<T>;
        using value_type = typename DetailType::value_type;
        using pointer = typename DetailType::pointer;
        using const_pointer = typename DetailType::pointer;
        using reference = typename DetailType::reference;
        using const_reference = typename DetailType::const_reference;
        using iterator = typename DetailType::iterator;
        using const_iterator = typename DetailType::const_iterator;
        using reverse_iterator = typename DetailType::reverse_iterator;
        using const_reverse_iterator = typename DetailType::const_reverse_iterator;

        iterator begin()
        {
            return data_.begin();
        }

        const_iterator begin() const
        {
            return data_.begin();
        }

        const_iterator cbegin() const
        {
            return data_.cbegin();
        }

        iterator end()
        {
            return data_.end();
        }

        const_iterator end() const
        {
            return data_.end();
        }

        const_iterator cend() const
        {
            return data_.cend();
        }

        reverse_iterator rbegin()
        {
            return data_.rbegin();
        }

        const_reverse_iterator rbegin() const
        {
            return data_.rbegin();
        }

        const_reverse_iterator crbegin() const
        {
            return data_.crbegin();
        }

        reverse_iterator rend()
        {
            return data_.rend();
        }

        const_reverse_iterator rend() const
        {
            return data_.rend();
        }

        const_reverse_iterator crend() const
        {
            return data_.crend();
        }

        constexpr Color() noexcept = default;

        constexpr Color(T r = T(), T g = T(), T b = T(), T a = T()) noexcept : data_({ r, g, b, a })
        {
        }

        constexpr Color(std::initializer_list<T> inilist) noexcept : data_(std::move(inilist))
        {

        }

        constexpr Color(const Color_T & other) : data_(other.data_)
        {
        }

        constexpr Color(Color_T && other) : data_(std::move(other))
        {

        }

        Color_T& operator=(std::initializer_list<T> inilist) noexcept
        {
            data_ = std::move(inilist);
            return *this;
        }

        Color_T& operator=(const Color_T & rhs) noexcept
        {
            data_ = rhs.data_;
            return *this;
        }

        Color_T& operator=(Color_T && rhs) noexcept
        {
            data_ = std::move(rhs.data_);
            return *this;
        }

        reference operator[](int index)
        {
            return data_[index];
        }

        constexpr const_reference operator[](int index) const
        {
            return data_[index];
        }

        reference R() noexcept
        {
            return data_[0];
        }

        constexpr const_reference R() const noexcept
        {
            return data_[0];
        }

        reference G() noexcept
        {
            return data_[1];
        }

        constexpr const_reference G() const noexcept
        {
            return data_[1];
        }

        reference B() noexcept
        {
            return data_[2];
        }

        constexpr const_reference B() const noexcept
        {
            return data_[2];
        }

        reference A() noexcept
        {
            return data_[3];
        }

        constexpr const_reference A() const noexcept
        {
            return data_[3];
        }

        constexpr std::tuple<uint8_t, uint8_t, uint8_t, uint8_t> RGBA8888() const noexcept
        {
            return std::make_tuple(static_cast<uint8_t>(Clamp(R(), T(1), T(1) * 255 + 0.5f)), 
                                   static_cast<uint8_t>(Clamp(G(), T(1), T(1) * 255 + 0.5f)),
                                   static_cast<uint8_t>(Clamp(B(), T(1), T(1) * 255 + 0.5f)), 
                                   static_cast<uint8_t>(Clamp(A(), T(1), T(1) * 255 + 0.5f)));
        }

        constexpr uint32_t ToUint32() const noexcept
        {
            auto rgba = RGBA8888();
            return ((std::get<0>(rgba)) << 24) | ((std::get<1>(rgba)) << 16) | 
                   ((std::get<2>(rgba)) <<  8) | ((std::get<3>(rgba)) <<  0);
        }

    private:
        DetailType data_;
    };

    using ColorF = Color<float>; 

    template<typename T>
    constexpr inline ColorF Clamp(const Color<T> & src) noexcept
    {
        ColorF dst;
        std::transform(src.begin(), src.end(), dst.begin(), [](typename Color<T>::const_reference val)
        {
            return Clamp(val / 255.0f, 0.0f, 1.0f);
        });
        return dst;
    }
}

#endif