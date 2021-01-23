#ifndef YTC_UNCOPYABLE_HPP
#define YTC_UNCOPYABLE_HPP

namespace YtcGE
{
    class Uncopyable
    {
    public:
        Uncopyable(const Uncopyable&) = delete;
        Uncopyable& operator=(const Uncopyable&) = delete;
        Uncopyable() = default;
        ~Uncopyable() = default;
    };
}

#endif