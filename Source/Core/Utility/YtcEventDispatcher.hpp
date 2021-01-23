#pragma once


#ifndef YTC_EVENT_DISPATCHER_HPP
#define YTC_EVENT_DISPATCHER_HPP

#include "../StringUtils/YtcString.hpp"
#include <vector>
#include <functional>
#include <unordered_map>
#include <type_traits>
#include <tuple>
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

    template<typename T>
    class EventDispatcher
    {
    public:
        using TagType = T;
        using HandlerName = String;
        using HandlerList = std::vector<SharedPtr<ICallable>>;
        EventDispatcher() noexcept
        {
        }

        ~EventDispatcher() noexcept
        {
        }

        template<typename...Args>
        void Dispatch(const T& tag,  Args&&...args) 
        {
            using HandlerType = EventHandler<Args...>;
            auto iter = eventMap_.find(tag);
            if (iter != eventMap_.end())
            {
                for (auto& handler : iter->second)
                {
                    (*std::static_pointer_cast<HandlerType>(handler))(std::forward<Args>(args)...);
                }
            }
        }

        template<typename...Args>
        void Register(std::function<void(Args...)> f, const T& tag)
        {
            eventMap_[tag].push_back(std::static_pointer_cast<ICallable>(MakeShared<EventHandler<Args...>>(std::move(f))));
        }

    private:
        std::unordered_map<T, HandlerList> eventMap_;
    };
}

#endif