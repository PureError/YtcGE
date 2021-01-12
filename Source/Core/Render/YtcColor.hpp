#ifndef YTC_COLOR_HPP
#define YTC_COLOR_HPP

#include "../Math/YtcVector.hpp"
namespace YtcGE
{
    template<typename T>
    struct Color
    {
        using Vector_T = Vector<T, 4>;
        using Color_T = Color<T>;
        using value_type = typename Vector_T::value_type;
        using pointer = typename Vector_T::pointer;
        using const_pointer = typename Vector_T::pointer;
        using reference = typename Vector_T::reference;
        using const_reference = typename Vector_T::const_reference;
        using iterator = typename Vector_T::iterator;
        using const_iterator = typename Vector_T::const_iterator;
        using reverse_iterator = typename Vector_T::reverse_iterator;
        using const_reverse_iterator = typename Vector_T::const_reverse_iterator;

        iterator begin()
        {
            return vec.begin();
        }

        const_iterator begin() const
        {
            return vec.begin();
        }

        const_iterator cbegin() const
        {
            return vec.cbegin();
        }

        iterator end()
        {
            return vec.end();
        }

        const_iterator end() const
        {
            return vec.end();
        }

        const_iterator cend() const
        {
            return vec.cend();
        }

        reverse_iterator rbegin()
        {
            return vec.rbegin();
        }

        const_reverse_iterator rbegin() const
        {
            return vec.rbegin();
        }

        const_reverse_iterator crbegin() const
        {
            return vec.crbegin();
        }

        reverse_iterator rend()
        {
            return vec.rend();
        }

        const_reverse_iterator rend() const
        {
            return vec.rend();
        }

        const_reverse_iterator crend() const
        {
            return vec.crend();
        }
        
        constexpr int Size() const noexcept
        {
            return vec.Size();
        }

        constexpr Color() noexcept = default;

        constexpr Color(T r, T g, T b, T a) noexcept : vec({ r, g, b, a })
        {
        }

        constexpr Color(std::initializer_list<T> inilist) noexcept : vec(std::move(inilist))
        {

        }

        constexpr Color(const Color_T & other) : vec(other.vec)
        {
        }

        constexpr Color(Color_T && other) : vec(std::move(other.vec))
        {

        }

        constexpr Color(const Vector_T & v) : vec(v)
        {
        }

        constexpr Color(Vector_T && v) : vec(std::move(v))
        {

        }

        Color_T& operator=(std::initializer_list<T> inilist) noexcept
        {
            vec = std::move(inilist);
            return *this;
        }

        Color_T& operator=(const Color_T & rhs) noexcept
        {
            vec = rhs.vec;
            return *this;
        }

        Color_T& operator=(Color_T && rhs) noexcept
        {
            vec = std::move(rhs.vec);
            return *this;
        }

        reference operator[](int index)
        {
            return vec[index];
        }

        constexpr const_reference operator[](int index) const
        {
            return vec[index];
        }

        reference R() noexcept
        {
            return vec[0];
        }

        constexpr const_reference R() const noexcept
        {
            return vec[0];
        }

        reference G() noexcept
        {
            return vec[1];
        }

        constexpr const_reference G() const noexcept
        {
            return vec[1];
        }

        reference B() noexcept
        {
            return vec[2];
        }

        constexpr const_reference B() const noexcept
        {
            return vec[2];
        }

        reference A() noexcept
        {
            return vec[3];
        }

        constexpr const_reference A() const noexcept
        {
            return vec[3];
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


        Color_T& operator+=(const Color_T & rhs) noexcept
        {
            this->vec += rhs.vec;
            return *this;
        }

        Color_T& operator-=(const Color_T & rhs) noexcept
        {
            this->vec -= rhs.vec;
            return *this;
        }


        Vector_T vec;
    };

    template<typename T>
    constexpr inline Color<T> operator+(const Color<T> & lhs, const Color<T> & rhs) noexcept
    {
        return lhs.vec + rhs.vec;
    }

    template<typename T>
    constexpr inline Color<T> operator-(const Color<T> & lhs, const Color<T> & rhs) noexcept
    {
        return lhs.vec - rhs.vec;
    }

    template<typename T, typename U>
    constexpr inline Color<T> operator*(const Color<T> & c, U s) noexcept
    {
        return c.vec * s;
    }

    template<typename T, typename U>
    constexpr inline Color<T> operator*(U s, const Color<T> & c) noexcept
    {
        return c * s;
    }

    template<typename T, typename U>
    constexpr inline Color<T> operator/(const Color<T> & c, U d) noexcept
    {
        return c.vec / d;
    }

    using ColorF = Color<float>; 
    using Color32b =  Color<uint8_t>;
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