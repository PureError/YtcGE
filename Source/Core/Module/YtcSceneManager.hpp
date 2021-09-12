#ifndef YTC_SCENE_MANAGER_HPP
#define YTC_SCENE_MANAGER_HPP

#include "../Entity/YtcScene.hpp"
#include "../Utility/YtcSingleton.hpp"
namespace YtcGE
{
    class SceneManager : public Singleton<SceneManager>
    {
        MARK_AS_SINGLETON();
    public:
        void AddScene(const ScenePtr& scene);
        void ReplaceScene(const String& name);
        void ReplaceScene() { (++current_scene_) %= SceneCount(); }
        ScenePtr CurrentScene() noexcept { return current_scene_ < 0 ? nullptr : scenes_[current_scene_]; }
        void Update();

        size_t SceneCount() noexcept
        {
            return scenes_.size();
        }
    private:
        using SceneList = std::vector<ScenePtr>;

        ScenePtr SceneByName(const String& name) noexcept
        {
            auto iter = std::find_if(scenes_.begin(), scenes_.end(), [&](const ScenePtr& s) { return s->Name() == name; });
            return iter == scenes_.end() ? nullptr : *iter;
        }

        SceneManager() = default;
        ~SceneManager() = default;
        int current_scene_ = -1;
        SceneList scenes_;
    };
}

#endif
