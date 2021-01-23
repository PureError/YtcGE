#include "YtcApplication.hpp"
#include "../Utility/Localization.hpp"
#include "../Render/YtcRenderer.hpp"
#include "../Entity/YtcSceneManager.hpp"
#include "../Entity/YtcCube.hpp"
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
    CreateSceneForTest();

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
            win_->Update();
            SceneManager::Instance().Update();
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

void YtcGE::Application::CreateSceneForTest()
{
    String scene_name = _T("TestScene");
    auto scene = MakeShared<Scene>(scene_name);
    CameraPtr cam = MakeShared<Camera>();
    cam->AdjustProjectionParam(DegreesToRadians(45.0f), 4.0f / 3, 0.1f, 1.0f);
    auto & s = *scene;
    std::array<Vec2f, Cube::VERTEX_COUNT> tex_coords;
    tex_coords[0] = { 0.1f, 0.2f };
    tex_coords[1] = { 0.8f, 0.4f };
    tex_coords[2] = { 0.9f, 0.4f };
    tex_coords[3] = { 0.5f, 0.5f };
    tex_coords[4] = { 0.7f, 0.1f };
    tex_coords[5] = { 0.3f, 0.6f };
    tex_coords[6] = { 0.6f, 0.9f };
    tex_coords[7] = { 0.2f, 0.8f };
    auto cube = MakeShared<Cube>(0.8f, 0.8f, 0.8f);
    cube->TextureCoord(tex_coords);
    auto img = Image::FromFile("./../../Resource/Images/windows_logo.bmp");
    cube->Texture(MakeShared<Texture2D>(img));
    //cube->Rotate(Vec3f{ 1.0f, 1.0f, 0.0f }, DegreesToRadians(45.0f));
    s.AddNode(cube);
    s.AddCamera(_T("TestCamera"), cam, true);
    SceneManager::Instance().AddScene(scene);
    SceneManager::Instance().ReplaceScence(scene_name);
}

