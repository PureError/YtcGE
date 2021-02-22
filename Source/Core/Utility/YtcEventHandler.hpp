#ifndef YTC_EVENT_HANDLER_HPP
#define YTC_EVENT_HANDLER_HPP

#include "../StringUtils/YtcString.hpp"
#include <type_traits>
#include <functional>

namespace YtcGE
{
    class ICallable
    {
    public:
        virtual void Invoke() = 0;
        virtual ~ICallable() {}
    };

    template<typename F, typename...Args, size_t...I>
    inline auto UnpackInvokeImpl(F&& f, std::tuple<Args...>& argsTuple, std::index_sequence<I...>)
        -> decltype (std::invoke(std::forward<F>(f), std::get<I>(argsTuple)...))
    {
        return std::invoke(std::forward<F>(f), std::get<I>(argsTuple)...);
    }

    template<typename F, typename...Args, size_t...I>
    inline auto UnpackInvokeImpl(F&& f, std::tuple<Args...>&& argsTuple, std::index_sequence<I...>)
        -> decltype (std::invoke(std::forward<F>(f), std::get<I>(std::move(argsTuple))...))
    {
        return std::invoke(std::forward<F>(f), std::get<I>(std::move(argsTuple))...);
    }

    struct InvokeHelper
    {
        template<typename F>
        static auto UnpackInvoke(F&& f, std::tuple<>& emptyTup)->decltype(std::invoke(std::forward<F>(f)))
        {
            return std::invoke(std::forward<F>(f));
        }

        template<typename F, typename...Args>
        static auto UnpackInvoke(F&& f, std::tuple<Args...>& nonEmptyTup)->decltype(UnpackInvokeImpl(f, nonEmptyTup, std::index_sequence_for<Args...>()))
        {
            return UnpackInvokeImpl(std::forward<F>(f), nonEmptyTup, std::index_sequence_for<Args...>());
        }
    };


    template<typename...Args>
    class EventHandler : public ICallable
    {
    public:
        using ArgsList = std::tuple<Args...>;
        using IndexSeq = std::index_sequence_for<Args...>;
        using Functor = std::function<void(Args...)>;
        EventHandler(const Functor& f) : func_(f)
        {
        }

        static constexpr int ArgsCount() noexcept
        {
            return sizeof...(Args);
        }

        void BindArgs(Args&&...args) noexcept
        {
            args_ = std::make_tuple<Args...>(std::forward<Args>(args)...);
        }

        void Invoke() override
        {
            InvokeHelper::UnpackInvoke(func_, args_);
        }

        void operator()(Args&&...args)
        {
            func_(std::forward<Args>(args)...);
        }
    protected:
        Functor func_;
        ArgsList args_;
    };
}


#endif