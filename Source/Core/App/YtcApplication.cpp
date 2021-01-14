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

    auto img = Image::FromFile("./../../Resource/Images/windows_logo.bmp");
    Triangle triangle;
    triangle.texture = std::make_shared<Texture2D>(img);
    auto v0 = make_shared<Vertex>();
    auto v1 = make_shared<Vertex>();
    auto v2 = make_shared<Vertex>();
    v0->position.X() = 200.0f;
    v0->position.Y() = 20.0f;
    v0->texcoord.U() = 0.0f;
    v0->texcoord.V() = 0.0f;
    v1->position.X() = 400.0f;
    v1->position.Y() = 300.0f;
    v1->texcoord.U() = 0.5f;
    v1->texcoord.V() = 0.5f;
    v2->position.X() = 100.0f;
    v2->position.Y() = 400.0f;
    v2->texcoord.U() = 1.0f;
    v2->texcoord.V() = 1.0f;
    triangle.vertices[0] = std::move(v0);
    triangle.vertices[1] = std::move(v1);
    triangle.vertices[2] = std::move(v2);

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
            //unsigned seed = chrono::system_clock::now().time_since_epoch().count();
            //std::mt19937 generator(seed);
            //auto w = win_->Width();
            //auto h = win_->Height();
            //uniform_int_distribution<int> distX(1, w);
            //uniform_int_distribution<int> distY(1, h);
            //const int pointCount = 100;
            //for (int i = 0; i < pointCount; ++i)
            //{
            //    Point2i pt{ distX(generator), distY(generator) };
            //    ColorF color{ 0.0f, 0.0f, 0.0f, 0.0f };
            //    Renderer::Get().DrawPoint(pt, color);
            //}

            //Renderer::Get().DrawLine2D({ 20, 20 }, { 300, 300 }, { 0.0f, 255.0f, 0.0f, 0.0f });
            //std::array<Point2i, 3> t;
            //t[0] = { 20, 20 };
            //t[1] = { 400, 300 };
            //t[2] = { 200, 500 };
            //Renderer::Get().DrawTriangle2D(t, { 0.0f, 0.0f, 255.0f, 0.0f });
            Renderer::Get().DrawTriangle(triangle);
            win_->Update();
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

