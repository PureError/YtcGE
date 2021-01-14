#include "YtcCamera.hpp"

namespace YtcGE
{

    void Camera::AdjustProjectionParam(float fov, float aspect, float near_plane, float far_plane)
    {
        fov_ = fov;
        aspect_ = aspect;
        near_plane_ = near_plane;
        far_plane_ = far_plane;

        proj_ = PerspectiveProjectionLH(fov, aspect, near_plane, far_plane);
        view_proj_ = V() * proj_;
    }

}