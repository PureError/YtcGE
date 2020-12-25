#ifndef _LOCALIZATION_HPP
#define _LOCALIZATION_HPP

#include <memory>
namespace YtcGE
{
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

}

#endif