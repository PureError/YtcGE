#ifndef YTC_APPLICATION_HPP
#define YTC_APPLICATION_HPP

#include "../StringUtils/YtcString.hpp"
#include "../Window/YtcWindow.hpp"

#include <vector>

namespace YtcGE
{

    class Application
    {
    public:
        Application();
        Application(const std::vector<String>& cmdArgs);
        void Run();
        void Terminate();
        ~Application();
    protected:
        WindowPtr MakeWindow() const;
        struct Impl;
        UniquePtr<Impl> impl_;
        std::vector<String> cmds_;
    };
}

#endif