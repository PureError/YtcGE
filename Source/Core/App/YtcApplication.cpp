#include <thread>
#include <random>
#include "YtcApplication.hpp"
#include "../Utility/Localization.hpp"
#include "../Render/YtcRenderer.hpp"
#include "../Module/YtcSceneManager.hpp"
#include "../Module/YtcGameLogicManger.hpp"
#include "../Entity/YtcCube.hpp"
#include "../Utility/YtcIniFile.hpp"


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
    auto msgPtr = msg.get();
    BOOL hasMsg = ::PeekMessage(msgPtr, NULL, 0, 0, PM_NOREMOVE);
    while (msgPtr->message != WM_QUIT)
    {
        if (win_->Active())
        {
            hasMsg = ::PeekMessage(msgPtr, NULL, 0, 0, PM_REMOVE);
        }
        else
        {
            hasMsg = ::GetMessage(msgPtr, NULL, 0, 0);
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


static SharedPtr<Node> CubeForTest()
{
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
    cube->TextureCoord(tex_coords);

    //cube->Rotate(Vec3f{ 1.0f, 1.0f, 0.0f }, DegreesToRadians(45.0f));
    return cube;
}

static SharedPtr<Node> DiabloForTest()
{
    auto node = MakeShared<Node>();

    return node;
}


void YtcGE::Application::CreateSceneForTest()
{
    IniObject setting(_T("./setting.ini"));
    {
        String scene_name = _T("test_node_cube");
        auto& section = setting.GetSection(scene_name);
        auto scene = MakeShared<Scene>(scene_name);
        auto camera = MakeShared<Camera>();
        auto w = win_->RenderBuffer().Width();
        auto h = win_->RenderBuffer().Height();
        auto aspect = w * 1.0f / h;
        camera->AdjustProjectionParam(DegreesToRadians(90.0f), aspect, 1.0f, 500.0f);
        scene->AddCamera(_T("TestCamera"), camera, true);
        auto cube = CubeForTest();
        cube->Name() = section.at(_T("name"));
        auto& txt_path = section.at(_T("texture_path"));
        if (!txt_path.empty())
        {
          auto img = Image::FromFile(txt_path);
          auto texture = MakeShared<Texture2D>(img);
          cube->Texture(texture);
        }
        scene->AddNode(cube);
        SceneManager::Instance().AddScene(scene);
        SceneManager::Instance().ReplaceScene(scene_name);
    }
    
    {
        String scene_name = _T("test_node_diablo");
        auto& section = setting.GetSection(scene_name);
        auto scene = MakeShared<Scene>(scene_name);
        auto camera = MakeShared<Camera>();
        auto w = win_->RenderBuffer().Width();
        auto h = win_->RenderBuffer().Height();
        auto aspect = w * 1.0f / h;
        camera->AdjustProjectionParam(DegreesToRadians(90.0f), aspect, 1.0f, 500.0f);
        scene->AddCamera(_T("TestCamera"), camera, true);
        auto node = DiabloForTest();
        ModelDataMapPtr model_data = MakeShared<ModelDataMap>(section.at(_T("model_path")));
        node->ModelInUse() = MakeShared<Model>(model_data);
        node->Name() = section.at(_T("name"));
        auto& txt_path = section.at(_T("texture_path"));
        if (!txt_path.empty())
        {
          auto texture = MakeShared<Texture2D>(Image::FromFile(txt_path));
          node->Texture(texture);
        }
        scene->AddNode(node);
        SceneManager::Instance().AddScene(scene);
    }
}

