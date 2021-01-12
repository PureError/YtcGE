#pragma once
#ifndef MATH_UTIL_HPP
#define MATH_UTIL_HPP


#include <cmath>
#include <numeric>
#include <limits>
#include <functional>
#include <algorithm>

namespace YtcGE
{
    template<typename T>
    constexpr inline T Abs(T val) noexcept
    {
        return std::abs(val);
    }


    template<typename A, typename B>
    constexpr inline auto Min(const A & a, const B & b) noexcept -> decltype(a < b ? a : b)
    {
        return a < b ? a : b;
    }

    template<typename A, typename B>
    constexpr inline auto Max(const A & a, const B & b) noexcept -> decltype(a > b ? a : b)
    {
        return a > b ? a : b;
    }

    template<typename T>
    constexpr inline bool NearlyEqual(T lhs, T rhs) noexcept
    {
        return Abs(lhs - rhs) <= std::numeric_limits<typename std::enable_if_t<std::is_floating_point<T>::value, T>>::epsilon();
    }

    constexpr float Pi = 3.14159265358f;

    template<typename T>
    constexpr inline std::common_type_t<float, T> AngleToRadians(T angle) noexcept
    {
        return angle * Pi / 180.0f;
    }

    template<typename T>
    constexpr inline std::common_type_t<float, T> RadiansToAngle(T radians) noexcept
    {
        return radians * 180.0f / Pi;
    }

    template<typename T>
    constexpr inline T Clamp(const T& val, const T& lowerbound, const T& upperbound) noexcept
    {
        return (std::max)(lowerbound, (std::min)(val, upperbound));
    }

    template<typename T>
    constexpr inline T Lerp(const T& lhs, const T& rhs, float s) noexcept
    {
        return lhs + (rhs - lhs) * s;
    }
}

#endif