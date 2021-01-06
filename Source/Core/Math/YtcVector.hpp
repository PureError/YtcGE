#pragma once
#ifndef YTC_VECTOR_HPP
#define YTC_VECTOR_HPP

#include "MathUtil.hpp"

#include <array>
namespace YtcGE
{
    template<typename T, int N>
    class Vector final
    {
        static_assert(std::is_arithmetic<T>::value, "can not use non-numeric type");
        
        template<typename U, int M>
        friend class Vector;

        using InternalStruct = std::array<T, N>;
    public:
        using Vector_T = Vector<T, N>;
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
            Fill(val);
        }

        constexpr Vector(std::initializer_list<T> initializer) noexcept
        {
            *this = std::move(initializer);
        }

        template<typename U, int M>
        constexpr Vector(const Vector<U, M>& other) noexcept
        {
            *this = other;
        }

        constexpr Vector(const Vector_T& other) noexcept : data_(other.data_)
        {
        }

        constexpr Vector(Vector_T&& other)  noexcept : data_(std::move(other.data_))
        {
        }

        Vector_T& operator=(std::initializer_list<T> initializer) 
        {
            std::copy(initializer.begin(), initializer.end(), begin());
            return *this;
        }

        Vector_T& operator=(const Vector_T&& other) noexcept
        {
            if (this != &other)
            {
                data_ = std::move(other.data_);
            }
            return *this;
        }

        template<typename U, int M>
        Vector_T& operator=(const Vector<U, M>& other) noexcept
        {
            constexpr int size = std::min(M, N);
            for (int i = 0; i < size; ++i)
            {
                data_[i] = static_cast<T>(other.data_[i]);
            }
            return *this;
        }


        Vector_T& operator=(const Vector_T& other) noexcept
        {
            if (this != &other)
            {
                std::copy(other.begin(), other.end(), begin());
            }
            return *this;
        }

        static const Vector_T& Zero() noexcept
        {
            static Vector_T zero(T(0));
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

        constexpr const_reference operator[](int i) const
        {
            return data_[i];
        }

        void Swap(Vector_T& other)
        {
            data_.swap(other.data_);
        }

        void Fill(T val) noexcept
        {
            data_.fill(val);
        }

        T& X()
        {
            static_assert(N >= 1, "Must be 1D at least!");
            return data_[0];
        }

        constexpr const T& X() const
        {
            static_assert(N >= 1, "Must be 1D at least!");
            return data_[0];
        }

        T& Y()
        {
            static_assert(N >= 2, "Must be 2D at least!");
            return data_[1];
        }

        constexpr const T& Y() const
        {
            static_assert(N >= 2, "Must be 2D at least!");
            return data_[1];
        }

        T& Z()
        {
            static_assert(N >= 3, "Must be 3D at least!");
            return data_[2];
        }

        constexpr const T& Z() const
        {
            static_assert(N >= 3, "Must be 3D at least!");
            return data_[2];
        }

        T& W()
        {
            static_assert(N >= 4, "Must be 4D at least!");
            return data_[3];
        }

        constexpr const T& W() const
        {
            static_assert(N >= 4, "Must be 4D at least!");
            return data_[3];
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

        constexpr operator const T*() const noexcept
        {
            return data_.data();
        }

        template<typename U>
        struct Rebind
        {
            using Type = Vector<U, N>;
        };

        template<typename U> Vector_T& operator+=(const Vector<U, N>& other) noexcept;
        template<typename U> Vector_T& operator-=(const Vector<U, N>& other) noexcept;
        template<typename U> Vector_T& operator*=(U scale) noexcept;
        template<typename U> Vector_T& operator/=(U divisor) noexcept;

        void Normalize() noexcept;
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
    inline Vector<T, N> operator*(const Vector<T, N>& lhs, U scale) noexcept
    {
        Vector<T, N> result;
        Mult(lhs, scale, result);
        return result;
    }

    template<typename T, typename U, int N>
    inline Vector<T, N> operator*(U scale, const Vector<T, N>& rhs) noexcept
    {
        return rhs * scale;
    }

    template<typename T, typename U, int N>
    inline Vector<T, N> operator/(const Vector<T, N>& lhs, U divisor) noexcept
    {
        Vector<T, N> result;
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

    template<typename T, typename U, int N>
    inline bool Perpendicular(const Vector<T, N>& lhs, const Vector<U, N>& rhs) noexcept
    {
        using ResultType = decltype(Dot(lhs, rhs));
        return NearlyEqual(Dot(lhs, rhs), ResultType(0));
    }

    template<typename T, typename U, int N>
    inline std::common_type_t<float, T, U> CosineValueOfAngleBetween(const Vector<T, N>& lhs, const Vector<U, N>& rhs) noexcept
    {
        using ResultType = std::common_type_t<float, T, U>;
        return Dot(lhs, rhs) / ResultType(Length(lhs) * Length(rhs));
    }

    template<typename T, typename U, int N>
    inline std::common_type_t<float, T, U> AngleBetween(const Vector<T, N>& lhs, const Vector<U, N>& rhs) noexcept
    {
        return std::acos(CosineValueOfAngleBetween(lhs, rhs));
    }

    template<typename T, int N>
    inline Vector<std::common_type_t<float, T>, N> Normalize(const Vector<T, N> & vec) noexcept
    {
        float len = (float)Length(vec);
        return vec / len;
    }


    template<typename T, int N>
    void YtcGE::Vector<T, N>::Normalize() noexcept
    {
        auto len = Length(*this);
        if (len)
        {
            (*this) /= len;
        }
    }

    using Vec2i = Vector<int, 2>;
    using Vec3i = Vector<int, 3>;
    using Vec4i = Vector<int, 4>;
    using Vec2f = Vector<float, 2>;
    using Vec3f = Vector<float, 3>;
    using Vec4f = Vector<float, 4>;


}


#endif
