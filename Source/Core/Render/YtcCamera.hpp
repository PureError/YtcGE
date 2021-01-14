#ifndef YTC_CAMERA_HPP
#define YTC_CAMERA_HPP

#include "../Math/YtcPoint.hpp"
#include "../Math/YtcRect.hpp"
#include "../Math/YtcMatrix.hpp"
#include <memory>
namespace YtcGE
{
    class Camera
    {
    public:
        Camera() noexcept
        {
        }

        ~Camera() noexcept
        {
        }

        float FOV() const noexcept
        {
            return fov_;
        }

        float Aspect() const noexcept
        {
            return aspect_;
        }

        float NearPlane() const noexcept
        {
            return near_plane_;
        }

        float FarPlane() const noexcept
        {
            return far_plane_;
        }

        const Mat44f& V() const noexcept
        {
            if (view_dirty_)
            {
                view_ = LookAtLH(Position(), Target(), UpDirection());
                view_proj_ = view_ * proj_;
                view_dirty_ = false;
            }
            return view_;
        }

        const Mat44f& P() const noexcept
        {
            return proj_;
        }

        const Mat44f& VP() const noexcept
        {
            return view_proj_;
        }

        void SetPosition(const Point3f& position) noexcept
        {
            position_ = position;
            view_dirty_ = true;
        }

        void SetUpDirection(const Point3f & up) noexcept
        {
            up_direction_ = up;
            view_dirty_ = true;
        }

        void SetTarget(const Point3f & target) noexcept
        {
            target_ = target;
            view_dirty_ = true;
        }

        const Point3f & Position() const noexcept
        {
            return position_;
        }

        const Vec3f & UpDirection() const noexcept
        {
            return up_direction_;
        }

        const Point3f & Target() const noexcept
        {
            return target_;
        }

        void AdjustProjectionParam(float fov, float aspect, float near_plane, float far_plane);

    private:
        float fov_;
        float aspect_;
        float near_plane_;
        float far_plane_;
        Point3f position_ = { -1.0f, 0.0f, 0.0f  };
        Vec3f up_direction_ = { 0.0f, 1.0f, 0.0f };
        Point3f target_ = { 0.0f, 0.0f, 0.0f };
        mutable Mat44f view_;
        mutable Mat44f proj_;
        mutable Mat44f view_proj_;
        mutable bool view_dirty_ = true;
    };
}

#endif