#ifndef YTC_SCENE_HPP
#define YTC_SCENE_HPP

#include "YtcNode.hpp"
#include "YtcCamera.hpp"
namespace YtcGE
{
    class Scene
    {
    public:
        Scene(const String & name) noexcept
            : name_(name)
        {
        }

        ~Scene() noexcept = default;

        const String& Name() const noexcept
        {
            return name_;
        }

        String & Name() noexcept
        {
            return name_;
        }

        const std::vector<SharedPtr<Node>>& Nodes() const noexcept
        {
            return nodes_;
        }

        SharedPtr<Node> NodeByName(const String& name) noexcept
        {
            auto iter = std::find_if(nodes_.begin(), nodes_.end(), [&](const SharedPtr<Node> & node)
            {
                return node->Name() == name;
            });

            return iter == nodes_.end() ? nullptr : *iter;
        }

        void AttachCamera(const String & tag) noexcept
        {
            auto iter = cameras_.find(tag);
            if (iter != cameras_.end())
            {
                camera_in_use_ = iter->second;
            }
        }

        CameraPtr CameraInUse() noexcept
        {
            return camera_in_use_;
        }

        bool AddCamera(const String& key, const CameraPtr& cam, bool use = false);
        bool AddNode(const SharedPtr<Node> & node)
        {
            if (!NodeByName(node->Name()))
            {
                nodes_.push_back(node);
                return true;
            }
            return false;
        }
    private:
        String name_;
        std::unordered_map<String, CameraPtr> cameras_;
        CameraPtr camera_in_use_;
        std::vector<SharedPtr<Node>> nodes_;
    };
    using ScenePtr = SharedPtr<Scene>;
}
#endif