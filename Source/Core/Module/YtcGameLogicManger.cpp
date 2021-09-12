#include "YtcGameLogicManger.hpp"
#include "YtcSceneManager.hpp"
#include "YtcInputManager.hpp"
#include "../App/YtcApplication.hpp"
namespace YtcGE
{
    GameLogicManager::GameLogicManager()
    {
      auto window = App->MainWindow();
#ifdef YTC_OS_WINDOWS
      window->AddEventListener(WM_KEYUP, ToStdFunction([this](Input::Device::Keyboard::KeyCode keycode) 
      {
          if (Input::Device::Keyboard::TranslateFromCode(keycode) == Input::Device::Keyboard::Key::Z) SceneManager::Instance().ReplaceScene();
      }));
#endif
    }

    void GameLogicManager::Update()
    {
        using namespace Input::Device;
        auto scene = SceneManager::Instance().CurrentScene();
        auto keyboard = InputManager::Instance().Keyboard();
        if (scene)
        {
            auto nodes = scene->Nodes();
            auto node = nodes.front();
            if (keyboard->IsKeyDown(Keyboard::Key::W))
            {
                node->Translate(Vec3f{ 0.0f, 0.0f, 0.005f });
            }
            else if(keyboard->IsKeyDown(Keyboard::Key::S))
            {
                node->Translate(Vec3f{ 0.0f, 0.0f, -0.005f });
            }
            else if (keyboard->IsKeyDown(Keyboard::Key::A))
            {
                node->Translate({ -0.005f , 0.0f, 0.0f });
            }
            else if (keyboard->IsKeyDown(Keyboard::Key::D))
            {
                node->Translate(Vec3f{ 0.005f, 0.0f, 0.0f });
            }
            else if (keyboard->IsKeyDown(Keyboard::Key::Q))
            {
                node->Rotate(Normalize(Vec3f{ 0.0f, 1.0f, 0.0f }), DegreesToRadians(0.1f));
            }
            else if (keyboard->IsKeyDown(Keyboard::Key::E))
            {
                node->Rotate(Normalize(Vec3f{ 0.0f, 1.0f, 0.0f }), DegreesToRadians(-0.1f));
            }

        }

    }

}