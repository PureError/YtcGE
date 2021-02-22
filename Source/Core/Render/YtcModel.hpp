#ifndef YTC_MESH_HPP
#define YTC_MESH_HPP

#include "../Fundation.hpp"
#include "../Math/YtcMatrix.hpp"
#include "YtcVertex.hpp"
#include "YtcTexture.hpp"


namespace YtcGE
{
    using VertexAttribList = std::vector<VertexAttribPtr>;
    using IndexTriad = std::array<int32_t, 3>;
    using IndexList = std::vector<IndexTriad>;

    struct ModelDataMap
    {
        ModelDataMap(const String& filepath);
        ~ModelDataMap() = default;

        struct FaceVtxAttrib
        {
            int idx_v;
            int idx_tex;
            int idx_norm;
        };

        using Face = std::vector<FaceVtxAttrib>;

        std::vector<Point3f> positions_v;
        std::vector<Vec3f> coords_tex;
        std::vector<Vec3f> vectors_norm;
        std::vector<Face> faces;

        template<typename OStream> void Dump(OStream & os) const;
    };

    template<typename OStream>
    void YtcGE::ModelDataMap::Dump(OStream & os) const
    {
        for (auto & pos : positions_v)
        {
            os << "\nv " << pos.X() << " " << pos.Y() << " " << pos.Z();
        }

        for (auto & coord_tex : coords_tex)
        {
            os << "\nvt " << coord_tex.U() << " " << coord_tex.V() << " " << coord_tex[2];
        }

        for (auto & vec_norm : vectors_norm)
        {
            os << "\nvn" << vec_norm.X() << " " << vec_norm.Y() << " " << vec_norm.Z();
        }

        for (auto & f : faces)
        {
            os << "\nf";
            for (auto & indices : f)
            {
                os << " " << indices.idx_v << '/';
                if (indices.idx_tex >= 0) os << indices.idx_tex;
                os << '/';
                if (indices.idx_norm >= 0) os << indices.idx_norm;
            }
        }

        os << std::endl;
    }

    using ModelDataMapPtr = SharedPtr<ModelDataMap>;

    class Model
    {
    public:
        using Ptr = SharedPtr<Model>;
        Model() noexcept = default;
        Model(const ModelDataMapPtr& mdata);
        void Rotate(const Vec3f& axis, float theta) noexcept;
        void Scale(const Vec3f& s) noexcept;
        void Transform(const Mat44f& mat) noexcept;
        const ModelDataMap* DataMap() const noexcept;
        VertexAttribList vertices;
        IndexList        indices;
        Texture2D::Ptr   texture;
    private:
        ModelDataMapPtr  data_map_;
    };

}

#endif