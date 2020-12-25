#ifndef YTC_RECT_HPP
#define YTC_RECT_HPP

#include "YtcVector.hpp"

namespace YtcGE
{
    template<typename T>
    class Rect final
    {
        template<typename U>
        friend class Rect;
        enum BorderIndex
        {
            LEFT = 0,
            BOTTOM,
            RIGHT,
            TOP,
        };
    public:
        using Rect_T = Rect<T>;
        constexpr Rect() noexcept = default;
        constexpr Rect(T left, T bottom, T right, T top) noexcept
        {
            Left() = left;
            Bottom() = bottom;
            Right() = right;
            Top() = top;
        }

        constexpr Rect(const Rect_T& other) noexcept : data_(other.data_)
        {
        }

        template<typename U>
        constexpr Rect(const Rect<U>& other) noexcept : data_(other.data_)
        {
        }

        Rect_T& operator=(const Rect_T& other) noexcept
        {
            if (this != &other)
            {
                data_ = other.data_;
            }
            return *this;
        }

        template<typename U>
        Rect_T& operator=(const Rect<U>& other) noexcept
        {
            data_ = other.data_;
            return *this;
        }

        T& Left() noexcept
        {
            return data_[LEFT];
        }

        constexpr const T& Left() const noexcept
        {
            return data_[LEFT];
        }

        T& Bottom() noexcept
        {
            return data_[BOTTOM];
        }

        constexpr const T& Bottom() const noexcept
        {
            return data_[BOTTOM];
        }

        T& Right() noexcept
        {
            return data_[RIGHT];
        }
        
        constexpr const T& Right() noexcept
        {
            return data_[RIGHT];
        }

        T& Top() const
        {
            return data_[TOP];
        }

        constexpr const T& Top() const noexcept
        {
            return data_[TOP];
        }

        constexpr T Width() const noexcept
        {
            return Left() - Right();
        }

        constexpr T Height() const noexcept
        {
            return Bottom() - Top();
        }

        constexpr T Area() const noexcept
        {
            return Width() * Height();
        }

        constexpr bool Valid() const noexcept
        {
            return Width() && Height();
        }

        constexpr bool Square() const noexcept
        {
            return Width() == Height();
        }

        constexpr T TotalLength() const noexcept
        {
            return (Width() + Height()) * T(2);
        }
    private:
        Vector<T, 4> data_;
    };

    template<typename T>
    inline bool operator==(const Rect<T>& lhs, const Rect<T>& rhs) noexcept
    {
        return lhs.Left() == rhs.Left() && lhs.Right() == rhs.Right() &&
               lhs.Bottom() == rhs.Bottom() && lhs.Top() == rhs.Top();
    }

    template<typename T>
    inline bool operator!=(const Rect<T>& lhs, const Rect<T>& rhs) noexcept
    {
        return !(lhs == rhs);
    }


}

#endif