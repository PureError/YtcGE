#ifndef YTC_LAMBDA_HPP
#define YTC_LAMBDA_HPP

#include <functional>
namespace YtcGE
{
    template<typename T>
    struct FunctionTraits
    {
    };

    template<typename C, typename R, typename...Args>
    struct FunctionTraits<R(__thiscall C::*)(Args...)>
    {
        using Functor = std::function<R(Args...)>;
    };

    template<typename C, typename R, typename...Args>
    struct FunctionTraits<R(__thiscall C::*)(Args...) const>
    {
        using Functor = std::function<R(Args...)>;
    };


    template<typename F>
    struct LambdaTraits
    {
        using Functor = typename FunctionTraits<decltype(&F::operator())>::Functor;
    };

    template<typename F>
    inline typename LambdaTraits<F>::Functor ToStdFunction(F lambda) noexcept
    {
        return lambda;
    }
}

#endif