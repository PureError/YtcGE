#pragma once


#ifndef YTC_EVENT_DISPATCHER_HPP
#define YTC_EVENT_DISPATCHER_HPP

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
    inline auto InvokeImpl(F&& f, std::tuple<Args...>&& argsTuple, std::index_sequence<I...>)
        -> decltype (std::invoke(std::forward<F>(f), std::get<I>(std::forward<std::tuple<Args...>>(argsTuple))...))
    {
        return std::invoke(std::forward<F>(f), std::get<I>(std::forward<std::tuple<Args...>>(argsTuple))...);
    }


    template<typename F, typename...Args>
    struct InvokeHelper
    {
        using ArgsTuple = std::tuple<Args...>;
        static auto Invoke(F& f, ArgsTuple& tup)
            ->decltype(InvokeImpl(std::forward<F>(f), std::forward<std::tuple<Args...>>(argsTuple), std::index_sequence_for<Args...>()))
        {
            return InvokeImpl(std::forward<F>(f), std::forward<std::tuple<Args...>>(argsTuple), std::index_sequence_for<Args...>());
        }
    };

    template<typename F>
    struct InvokeHelper<F>
    {
        using ArgsTuple = std::tuple<>;
        static auto Invoke(F& f, ArgsTuple& tup)
            ->decltype(std::invoke(std::forward<F>(f)))
        {
            return std::invoke(std::forward<F>(f));
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
            InvokeHelper<Functor, Args...>::Invoke(func_, args_);
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
        using HandlerList = std::unordered_map<HandlerName, SharedPtr<ICallable>>;
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
                    (*std::static_pointer_cast<HandlerType>(handler.second))(std::forward<Args>(args)...);
                }
            }
        }

        template<typename...Args>
        void Register(std::function<void(Args...)> f, const T& tag, const HandlerName& name)
        {
            eventMap_[tag][name] = std::static_pointer_cast<ICallable>(MakeShared<EventHandler<Args...>>(std::move(f)));
        }

    private:
        std::unordered_map<T, HandlerList> eventMap_;
    };
}

#endif