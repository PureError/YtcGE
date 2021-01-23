#ifndef YTC_SINGLETON_HPP
#define YTC_SINGLETON_HPP

#include "Localization.hpp"
#include "YtcUncopyable.hpp"
#include "YtcSyncObject.hpp"

namespace YtcGE
{
    template<class T, class Mutex = null_mutex, class Deleter = typename ::std::default_delete<T>>
    class Singleton : protected Uncopyable
    {
    public:
        Singleton(Singleton<T>&&) = delete;
        Singleton<T>& operator=(Singleton<T>&&) = delete;

        static T& Instance()
        {
            static UniquePtr<T> inst;
            if (!inst)
            {
                static Mutex mutex;
                mutex.lock();
                if (!inst)
                {
                    inst.reset(new T());
                }
                mutex.unlock();
            }
            return *inst;
        }
    protected:
        using Deleter_T= Deleter;
        using Mutex_T = Mutex;
        using Singleton_T = Singleton<T, Mutex, Deleter>;

        Singleton() noexcept= default;
        ~Singleton() noexcept = default;
    };

#define MARK_AS_SINGLETON()\
friend class Singleton_T;\
friend struct Deleter_T
}

#endif