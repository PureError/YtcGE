#ifndef _LOCALIZATION_HPP
#define _LOCALIZATION_HPP

#include "../StringUtils/YtcString.hpp"
#include "YtcLambda.hpp"
#include <memory>
#include <cassert>
#define YTC_RT_ASSERT assert
#define YTC_STATIC_ASSERT static_assert
namespace YtcGE
{
    using Byte = uint8_t;
    using Word = uint16_t;
    using DWord = uint32_t;

    template<typename T>
    using SharedPtr = std::shared_ptr<T>;
    template<typename T>
    using UniquePtr = std::unique_ptr<T>;


    template<typename T, typename...Args>
    inline SharedPtr<T> MakeShared(Args&&...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    template<typename T, typename...Args>
    inline UniquePtr<T> MakeUnique(Args&&...args)
    {
        return UniquePtr<T>(new T(std::forward<Args>(args)...));
    }

    template<typename To, typename From>
    inline SharedPtr<To> CastPtr(const SharedPtr<From> & from)
    {
        YTC_RT_ASSERT(dynamic_cast<To*>(from.get()) == from.get());
        return std::static_pointer_cast<To>(from);
    }

}

#endif