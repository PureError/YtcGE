#pragma once
#ifndef YTC_VECTOR_HPP
#define YTC_VECTOR_HPP

#include "MathUtil.hpp"

#include <array>
namespace YtcGE
{
    template<typename T, int N>
    class Vector
    {
        static_assert(std::is_arithmetic<T>::value, "can not use non-numeric type");
        using InternalStruct = std::array<T, N>;
    public:
        using Vector_t = Vector<T, N>;
        using value_type = typename InternalStruct::value_type;
        using difference_type = typename InternalStruct::difference_type;
        using pointer = typename InternalStruct::pointer;
        using reference = typename InternalStruct::reference;
        using const_reference = typename InternalStruct::const_reference;
        using iterator = typename InternalStruct::iterator;
        using const_iterator = typename InternalStruct::const_iterator;
        using reverse_iterator = typename InternalStruct::reverse_iterator;
        using const_reverse_iterator = typename InternalStruct::const_reverse_iterator;

        constexpr Vector() = default;

        constexpr Vector(value_type val) noexcept
        {
            data_.fill(val);
        }

        constexpr Vector(std::initializer_list<T> initializer) noexcept
        {
            *this = std::move(initializer);
        }

        template<typename U, int S>
        constexpr Vector(const Vector<U, S>& other) noexcept
        {
            std::copy(other.begin(), other.begin() + std::min(Size(), other.Size()), begin());
        }

        constexpr Vector(const Vector_t& other) noexcept : data_(other.data_)
        {
        }

        constexpr Vector(Vector_t&& other)  noexcept : data_(std::move(other.data_))
        {
        }

        Vector_t& operator=(std::initializer_list<T> initializer) 
        {
            std::copy(initializer.begin(), initializer.end(), begin());
            return *this;
        }

        Vector_t& operator=(const Vector_t&& other) noexcept
        {
            if (this != &other)
            {
                data_ = std::move(other.data_);
            }
            return *this;
        }

        template<typename U, int S>
        Vector& operator=(const Vector<U, S>& other) noexcept
        {
            std::copy(other.begin(), other.begin() + std::min(Size(), other.Size()), begin());
            return *this;
        }

        Vector_t& operator=(const Vector_t& other) noexcept
        {
            if (this != &other)
            {
                std::copy(other.begin(), other.end(), begin());
            }
            return *this;
        }

        static const Vector_t& Zero() noexcept
        {
            static Vector_t zero(T(0));
            return zero;
        }

        iterator begin() noexcept
        {
            return data_.begin();
        }

        const_iterator begin() const noexcept
        {
            return data_.begin();
        }

        const_iterator cbegin() const noexcept
        {
            return begin();
        }

        iterator end() noexcept
        {
            return data_.end();
        }

        const_iterator end() const noexcept
        {
            return data_.end();
        }

        const_iterator cend() const noexcept
        {
            return end();
        }

        reverse_iterator rbegin() noexcept
        {
            return data_.rbegin();
        }

        const_reverse_iterator rbegin() const noexcept
        {
            return data_.rbegin();
        }

        const_reverse_iterator crbegin() const noexcept
        {
            return rbegin();
        }
        
        reverse_iterator rend() noexcept
        {
            return data_.rend();
        }

        const_reverse_iterator rend() const noexcept
        {
            return data_.rend();
        }

        const_reverse_iterator crend() const noexcept
        {
            return rend();
        }

        constexpr int Size() const noexcept
        {
            return (int)data_.size();
        }

        reference operator[](int i)
        {
            return data_[i];
        }

        const_reference operator[](int i) const
        {
            return data_[i];
        }

        void Swap(Vector_t& other)
        {
            data_.swap(other.data_);
        }

        template<typename U>
        constexpr bool NearlyEqualTo(const Vector<U, N>& other) const noexcept
        {
            using ElemType = std::common_type_t<T, U>;
            return std::equal(cbegin(), cend(), other.cbegin(), other.cend(), NearlyEqual<ElemType>);
        }

        operator T*() noexcept
        {
            return data_.data();
        }

        operator const T*() const noexcept
        {
            return data_.data();
        }

        template<typename U>
        struct Rebind
        {
            using Type = Vector<U, N>;
        };

        template<typename U> Vector_t& operator+=(const Vector<U, N>& other) noexcept;
        template<typename U> Vector_t& operator-=(const Vector<U, N>& other) noexcept;
        template<typename U> Vector_t& operator*=(U scale) noexcept;
        template<typename U> Vector_t& operator/=(U divisor) noexcept;
    private:
        InternalStruct data_;
    };

    template<typename T, int N>
    constexpr inline bool operator==(const Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept
    {
        return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
    }

    template<typename T, int N>
    constexpr inline bool operator!=(const Vector<T, N>& lhs, const Vector<T, N>& rhs) noexcept
    {
        return !(lhs == rhs);
    }

    template<typename T, typename U, typename C, int N>
    inline void Add(const Vector<T, N>& lhs, const Vector<U, N>& rhs, Vector<C, N>& result) noexcept
    {
        for (int i = 0; i < N; ++i)
        {
            result[i] = static_cast<C>(lhs[i] + rhs[i]);
        }
    }

    template<typename T, typename U, typename C, int N>
    inline void Sub(const Vector<T, N>& lhs, const Vector<U, N>& rhs, Vector<C, N>& result) noexcept
    {
        for (int i = 0; i < N; ++i)
        {
            result[i] = static_cast<std::common_type_t<T, U>>(lhs[i] - rhs[i]);
        }
    }

    template<typename T, typename U, typename C, int N>
    inline void Mult(const Vector<T, N>& lhs, U scale, Vector<C, N>& result) noexcept
    {
        static_assert(std::is_arithmetic<U>::value, "the type of <scale> should be numeric");
        for (int i = 0; i < N; ++i)
        {
            result[i] = static_cast<C>(lhs[i] * scale);
        }
    }

    template<typename T, typename U, typename C, int N>
    inline void Div(const Vector<T, N>& lhs, U divisor, Vector<C, N>& result) noexcept
    {
        auto scale = 1.0f / divisor;
        Mult(lhs, scale, result);
    }

    template<typename T, typename U, int N>
    inline Vector<std::common_type_t<T, U>, N> operator+(const Vector<T, N>& lhs, const Vector<U, N>& rhs) noexcept
    {
        using FinalType = std::common_type_t<T, U>;
        Vector<FinalType, N> result;
        Add(lhs, rhs, result);
        return result;
    }

    template<typename T, typename U, size_t N>
    inline Vector<std::common_type_t<T, U>, N> operator-(const Vector<T, N>& lhs, const Vector<U, N>& rhs) noexcept
    {
        using FinalType = std::common_type_t<T, U>;
        Vector<FinalType, N> result;
        Sub(lhs, rhs, result);
        return result;
    }

    template<typename T, typename U, int N>
    inline Vector<std::common_type_t<T, U>, N> operator*(const Vector<T, N>& lhs, U scale) noexcept
    {
        using FinalType = std::common_type_t<T, U>;
        Vector<FinalType, N> result;
        Mult(lhs, scale, result);
        return result;
    }

    template<typename T, typename U, int N>
    inline Vector<std::common_type_t<T, U>, N> operator*(U scale, const Vector<T, N>& rhs) noexcept
    {
        return rhs * scale;
    }

    template<typename T, typename U, int N>
    inline Vector<typename std::common_type_t<float, std::common_type_t<T, typename std::enable_if_t<std::is_arithmetic<U>::value, U>>>, N>
                     operator/(const Vector<T, N>& lhs, U divisor) noexcept
    {
        Vector<typename std::common_type_t<float, std::common_type_t<T, typename std::enable_if_t<std::is_arithmetic<U>>>>, N> result;
        Div(lhs, divisor, result);
        return result;
    }

    template<typename T, int N>
    template<typename U>
    inline Vector<T, N>& Vector<T, N>::operator+=(const Vector<U, N>& other) noexcept
    {
        Add(*this, other, *this);
        return *this;
    }

    template<typename T, int N>
    template<typename U>
    inline Vector<T, N>& Vector<T, N>::operator-=(const Vector<U, N>& other) noexcept
    {
        Sub(*this, other, *this);
        return *this;
    }

    template<typename T, int N>
    template<typename U>
    inline Vector<T, N>& Vector<T, N>::operator*=(U scale) noexcept
    {
        Mult(*this, scale, *this);
        return *this;
    }

    template<typename T, int N>
    template<typename U>
    inline Vector<T, N>& Vector<T, N>::operator/=(U divisor) noexcept
    {
        Div(*this, divisor, *this);
        return *this;
    }

    template<typename T, typename U, int N>
    inline std::common_type_t<T, U> Dot(const Vector<T, N>& lhs, const Vector<U, N>& rhs) noexcept
    {
        std::common_type_t<T, U> result{};
        for (int i = 0; i < N; ++i)
        {
            result += (lhs[i] * rhs[i]);
        }
        return result;
    }

    template<typename T, int N>
    inline T LengthSquared(const Vector<T, N>& vec) noexcept
    {
        T result {};
        for (auto e : vec)
        {
            result += e * e;
        }
        return result;
    }

    template<typename T, int N>
    inline T Length(const Vector<T, N>& vec) noexcept
    {
        return std::sqrt(LengthSquared(vec));
    }

    using Vec2i = Vector<int, 2>;
    using Vec3i = Vector<int, 3>;
    using Vec4i = Vector<int, 4>;
    using Vec2f = Vector<float, 2>;
    using Vec3f = Vector<float, 3>;
    using Vec4f = Vector<float, 4>;

}


#endif
