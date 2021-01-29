#ifndef YTC_MESH_HPP
#define YTC_MESH_HPP

#include "../Fundation.hpp"
#include "../Math/YtcMatrix.hpp"
#include "YtcVertex.hpp"
#include "YtcTexture.hpp"

namespace YtcGE
{
    using VertexList = std::vector<VertexPtr>;
    using IndexTriad = std::array<int32_t, 3>;
    using IndexList = std::vector<IndexTriad>;

    class Model
    {
    public:
        void Rotate(const Vec3f& axis, float theta) noexcept;
        void Scale(const Vec3f& s) noexcept;
        void Transform(const Mat44f& mat) noexcept;
        
        template<typename Stream>
        void Dump(Stream & s) const noexcept
        {
            for (size_t i = 0; i < vertices.size(); ++i)
            {
                s << "vtx " << i << ":";
                vertices[i]->Dump(s);
            }
        }
        VertexList     vertices;
        IndexList      indices;
        Texture2D::Ptr texture;
    };

}

#endif