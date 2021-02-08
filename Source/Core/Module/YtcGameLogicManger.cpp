#include "YtcGameLogicManger.hpp"
#include "YtcSceneManager.hpp"
#include "YtcInputManager.hpp"
namespace YtcGE
{

    void GameLogicManager::Update()
    {
        using namespace Input::Device;
        auto scene = SceneManager::Instance().CurrentScene();
        if (scene)
        {
            auto node = scene->NodeByName(_T("YtcCube"));
            auto keyboard = InputManager::Instance().Keyboard();

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