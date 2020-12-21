#pragma once
#ifndef MATH_UTIL_HPP
#define MATH_UTIL_HPP

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

    template<typename T>
    constexpr inline bool NearlyEqual(T lhs, T rhs) noexcept
    {
        return Abs(lhs - rhs) <= std::numeric_limits<typename std::enable_if_t<std::is_floating_point<T>::value, T>>::epsilon();
    }
}

#endif