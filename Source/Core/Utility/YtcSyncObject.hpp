#ifndef YTC_SYNC_OBJECT_HPP
#define YTC_SYNC_OBJECT_HPP

#include <mutex>
namespace YtcGE
{
    class null_mutex
    {
    public:
        void lock() const noexcept
        {
        }

        bool try_lock() const noexcept
        {
            return true;
        }

        void unlock() const noexcept
        {
        }
    };
}


#endif