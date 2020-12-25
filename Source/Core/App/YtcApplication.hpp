#ifndef YTC_APPLICATION_HPP
#define YTC_APPLICATION_HPP

#include "../StringUtils/YtcString.hpp"

namespace YtcGE
{

    class Application
    {
    public:
        Application(const String& name);
        virtual void Run() {}
        virtual ~Application() {}
    protected:
        String name_;
        float fps_;
    };
}

#endif