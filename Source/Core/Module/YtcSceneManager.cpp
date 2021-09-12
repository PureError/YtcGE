#include "YtcSceneManager.hpp"
#include "../Utility/YtcException.hpp"
#include "../Render/YtcRenderer.hpp"
#include "../Module/YtcInputManager.hpp"
#include "../Log/YtcLog.hpp"
#include <iostream>
namespace YtcGE
{
    void SceneManager::AddScene(const ScenePtr& scene)
    {
        if (!SceneByName(scene->Name()))
        {
            scenes_.push_back(scene);
        }
        else
        {
            StringStream ss;
            ss << _T("Scene ") << scene->Name() << _T("existed already!");
            throw Exception(ss.str());
        }
    }

    void SceneManager::ReplaceScene(const String& name)
    {
        auto iter = std::find_if(scenes_.begin(), scenes_.end(), [&](const ScenePtr& s) { return s->Name() == name; });
        if (iter != scenes_.end())
        {
            current_scene_ = (iter - scenes_.begin());
        }
    }


    void SceneManager::Update()
    {
        auto keyboard = InputManager::Instance().Keyboard();
        Renderer::Get().BackGround(ColorF());
        auto scene_ptr = CurrentScene();
        if (scene_ptr)
        {
            auto& s = *scene_ptr;
            // update the elements in current scene
            // render nodes
            auto & cam = s.CameraInUse();
            auto & mat_vp = cam->VP();
            auto & nodes = s.Nodes();
            for (auto & n : nodes)
            {
                //n->Rotate(Normalize(Vec3f{ 0.0f, 1.0f, 0.0f }), DegreesToRadians(10.0f));
                auto & model = n->ModelInUse();
                if (model)
                {
                    auto & mvp = n->Model2Word() * mat_vp;
                    Renderer::Get().SetTransformMatrix(mvp);
                    Renderer::Get().DrawModel(*model);
                }
            }

        }
    }

}