#include "YtcModel.hpp"



namespace YtcGE
{
    void Model::Rotate(const Vec3f& axis, float theta) noexcept
    {
        this->Transform( _3DTransformHelper<float>::Rotatation(axis, theta));
    }

    void Model::Scale(const Vec3f& s) noexcept
    {
        this->Transform(_3DTransformHelper<float>::Scaling(s[0], s[1], s[2]));
    }

    void Model::Transform(const Mat44f & mat) noexcept
    {
        for (auto& vtx : vertices)
        {
            (vtx->position) *= mat;
        }
    }

}
