#include "YtcScene.hpp"

namespace YtcGE
{

    bool Scene::AddCamera(const String& key, const CameraPtr& cam, bool use /*= false*/)
    {
        auto& value = cameras_[key];
        if (!value)
        {
            value = cam;
            if (use)
            {
                camera_in_use_ = cam;
            }
            return true;
        }
        return false;
    }

}