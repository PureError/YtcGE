#include "YtcApplication.hpp"
#include "../Utility/Localization.hpp"
#include "../Render/YtcRenderer.hpp"
#include "../Module/YtcSceneManager.hpp"
#include "../Module/YtcGameLogicManger.hpp"
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
            GameLogicManager::Instance().Update();
            SceneManager::Instance().Update();
            std::this_thread::sleep_for(chrono::milliseconds(1));
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
    auto w = win_->RenderBuffer().Width();
    auto h = win_->RenderBuffer().Height();
    auto aspect = w * 1.0f / h;
    cam->AdjustProjectionParam(DegreesToRadians(90.0f), aspect, 1.0f, 500.0f);
    auto & s = *scene;
    std::array<Vec2f, Cube::VERTEX_COUNT> tex_coords;
    tex_coords[Cube::FAR_TOP_RIGHT] = { 1.0f, 0.0f };
    tex_coords[Cube::FAR_TOP_LEFT] = { 0.0f, 0.0f };
	tex_coords[Cube::FAR_BOTTOM_LEFT] = { 0.0f, 1.0f };
	tex_coords[Cube::FAR_BOTTOM_RIGHT] = { 1.0f, 1.0f };
    tex_coords[Cube::NEAR_TOP_RIGHT] = { 1.0f, 0.0f };
    tex_coords[Cube::NEAR_TOP_LEFT] = { 0.0f, 0.0f };
    tex_coords[Cube::NEAR_BOTTOM_LEFT] = { 0.0f, 1.0f };
    tex_coords[Cube::NEAR_BOTTOM_RIGHT] = { 1.0f, 1.0f };
    auto cube = MakeShared<Cube>(2.0f, 2.0f, 2.0f);
    cube->Name() = _T("YtcCube");
    cube->TextureCoord(tex_coords);
    try
    {
        auto img = Image::FromFile("./../../Resource/Images/windows_logo.bmp");
        auto texture = MakeShared<Texture2D>(img);
        cube->Texture(texture);
    }
    catch (...)
    {

    }

    //cube->Rotate(Vec3f{ 1.0f, 1.0f, 0.0f }, DegreesToRadians(45.0f));
    s.AddNode(cube);
    s.AddCamera(_T("TestCamera"), cam, true);
    SceneManager::Instance().AddScene(scene);
    SceneManager::Instance().ReplaceScence(scene_name);
}

