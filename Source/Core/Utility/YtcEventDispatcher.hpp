#pragma once


#ifndef YTC_EVENT_DISPATCHER_HPP
#define YTC_EVENT_DISPATCHER_HPP
#include "YtcEventHandler.hpp"
#include <vector>
#include <unordered_map>
#include <tuple>
namespace YtcGE
{


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