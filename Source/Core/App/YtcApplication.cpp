#include "YtcApplication.hpp"
#include "../Utility/Localization.hpp"
#include "../Render/YtcRenderer.hpp"
#include <thread>
#include <random>


using namespace YtcGE;
using namespace std;

static inline YtcGE::WindowPtr MakeWindow() 
{
    return MakeShared<BasicWindow>(BasicWindow::Attributes());
}

YtcGE::UniquePtr<YtcGE::Application> YtcGE::Application::Create(const std::vector<String> & cmdArgs)
{
    return MakeUnique<YtcGE::Application>(MakeUnique<Application::RuntimeInfo>(cmdArgs));
}

void YtcGE::Application::Run()
{
    win_ = MakeWindow();
    UniquePtr<MSG> msg = MakeUnique<MSG>();
    auto hwnd = win_->Handle();
    auto msgPtr = msg.get();
    BOOL hasMsg = ::PeekMessage(msgPtr, hwnd, 0, 0, PM_NOREMOVE);
    while (msgPtr->message != WM_QUIT)
    {
        if (win_->Active())
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
            //render
            unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            std::mt19937 generator(seed);
            auto w = win_->Width();
            auto h = win_->Height();
            uniform_int_distribution<int> distX(1, w);
            uniform_int_distribution<int> distY(1, h);
            const int pointCount = 100;
            for (int i = 0; i < pointCount; ++i)
            {
                Point2i pt{ distX(generator), distY(generator) };
                ColorF color{ 0.0f, 0.0f, 0.0f, 0.0f };
                Renderer::Get().DrawPoint(pt, color);
            }

            //Renderer::Get().DrawLine2D({ 20, 20 }, { 300, 300 }, { 0.0f, 255.0f, 0.0f, 0.0f });

            std::array<Point2i, 3> t;
            t[0] = { 20, 20 };
            t[1] = { 400, 300 };
            t[2] = { 200, 500 };
            Renderer::Get().DrawTriangle2D(t, { 0.0f, 0.0f, 255.0f, 0.0f });
            std::this_thread::sleep_for(chrono::microseconds(30));
        }
    }

}



YtcGE::Application::Application(UniquePtr<RuntimeInfo>&& runtimeInfo) : rtInfo_(std::move(runtimeInfo))
{

}

YtcGE::Application::~Application()
{
}

