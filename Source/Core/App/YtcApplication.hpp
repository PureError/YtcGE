#ifndef YTC_APPLICATION_HPP
#define YTC_APPLICATION_HPP

#include "../StringUtils/YtcString.hpp"
#include "../Window/YtcWindow.hpp"

#include <vector>

namespace YtcGE
{

    class Application
    {
        struct RuntimeInfo
        {
            RuntimeInfo(const std::vector<String>& args) : cmdArgs(args)
            {
            }
            const std::vector<String> cmdArgs;
        };

        using RuntimeInfoPtr = UniquePtr<RuntimeInfo>;
    public:
        Application(const Application&) = delete;
        Application(Application&&) = delete;
        Application& operator=(const Application&) = delete;
        Application& operator=(Application&&) = delete;


        static UniquePtr<Application> Create(const std::vector<String> & cmdArgs);

        WindowPtr MainWindow() noexcept
        {
            return win_;
        }

        void Run();
        explicit Application(UniquePtr<RuntimeInfo>&& runtimeInfo);
        ~Application();
    private:
        WindowPtr win_;
        RuntimeInfoPtr rtInfo_;
    };

}

#endif