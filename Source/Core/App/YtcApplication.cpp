#include "YtcApplication.hpp"
#include "../Window/YtcWindow.hpp"
#include "../Utility/Localization.hpp"
#include <thread>


using namespace YtcGE;
struct YtcGE::Application::Impl
{
    WindowPtr window;
};


static inline YtcGE::WindowPtr MakeWindow() 
{
    return MakeShared<BasicWindow>(BasicWindow::Attributes());
}


YtcGE::Application::Application(const std::vector<String>& cmdArgs) : cmds_(cmdArgs), impl_(new Impl())
{

}

YtcGE::Application::Application() : impl_(new Impl())
{

}

void YtcGE::Application::Run()
{
    impl_->window = MakeWindow();
    UniquePtr<MSG> msg = MakeUnique<MSG>();
    auto hwnd = impl_->window->Handle();
    auto msgPtr = msg.get();
    BOOL hasMsg = ::PeekMessage(msgPtr, hwnd, 0, 0, PM_NOREMOVE);
    while (msgPtr->message != WM_QUIT)
    {
        if (impl_->window->Active())
        {
            hasMsg = ::PeekMessage(msgPtr, hwnd, 0, 0, PM_REMOVE);
        }
        else
        {
            hasMsg = ::GetMessage(msgPtr, hwnd, 0, 0);
        }

        if (hasMsg)
        {
            ::TranslateMessage(msgPtr);
            ::DispatchMessage(msgPtr);
        }
        else
        {
            //‰÷»æ
        }
    }

}



YtcGE::Application::~Application()
{
}

