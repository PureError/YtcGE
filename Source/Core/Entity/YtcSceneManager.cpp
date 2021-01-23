#include "YtcSceneManager.hpp"
#include "../Utility/YtcException.hpp"
#include "../Render/YtcRenderer.hpp"
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

    void SceneManager::ReplaceScence(const String& name)
    {
        auto s = SceneByName(name);
        if (s)
        {
            current_scene_ = s;
        }
    }


    void SceneManager::Update()
    {
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
                auto & model_data = n->ModelData();
                auto & mvp = n->Model2Word() * mat_vp;
                Renderer::Get().SetTransformMatrix(mvp);
                Renderer::Get().DrawModel(model_data);
            }

        }
    }

}