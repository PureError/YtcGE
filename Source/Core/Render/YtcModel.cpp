#include "YtcModel.hpp"
#include "../Utility/YtcException.hpp"
#include "../Utility/YtcFileStream.hpp"

namespace YtcGE
{


    Model::Model(const ModelDataMapPtr& mdata)
        : data_map_(mdata)
    {
        YTC_RT_ASSERT(data_map_ != nullptr);
        const ModelDataMap & data = *data_map_;
        
        int n = 0;
        for (auto& f : data.faces)
        {
            YTC_RT_ASSERT(f.size() == 3);
            for (auto& indices : f)
            {
                VertexAttribPtr vtx_attrib = MakeShared<VertexAttrib>();
                vtx_attrib->position = IncDimension(data.positions_v[indices.idx_v - 1]);
                auto & tc = data.coords_tex[indices.idx_tex - 1];
                vtx_attrib->texcoord = { tc.U() / tc.W(), tc.V() / tc.W() };
                this->vertices.push_back(std::move(vtx_attrib));
            }
            IndexTriad idx_traid;
            int32_t idx = n * 3;
            for (int i = 0; i < 3; ++i) idx_traid[i] = idx++;
            this->indices.push_back(idx_traid);
            ++n;
        }
    }

    void Model::Rotate(const Vec3f& axis, float theta) noexcept
    {
        this->Transform( _3DTransformHelper<float>::Rotation(axis, theta));
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

    const YtcGE::ModelDataMap* Model::DataMap() const noexcept
    {
        return data_map_.get();
    }

    ModelDataMap::ModelDataMap(const String& filepath)
    {
        std::ifstream fs(filepath);
        if (!fs.is_open())
        {
            throw Exception(_T("failed to open file!"));
        }
        AString line_rall_txt;
        while (!fs.eof())
        {
            std::getline(fs, line_rall_txt);
            auto line = TrimLeft(line_rall_txt);
            if (!line.empty())
            {
                if (line.front() != '#')
                {
                    std::istringstream is(line);
                    auto prefix = line.substr(0, 2);
                    if (prefix == "v ")
                    {
                        is.seekg(2);
                        Vec3f pos;
                        for (int i = 0; i < pos.Size(); ++i)
                        {
                            is >> pos[i];
                        }
                        positions_v.push_back(pos);
                    }
                    else if (prefix == "vt")
                    {
                        is.seekg(2);
                        float u, v, w = 1.0f;
                        is >> u >> v >> w;
                        Vec3f tc{ u, v, w };
                        coords_tex.push_back(tc);
                    }
                    else if (prefix == "vn")
                    {
                        is.seekg(2);
                        Vec3f norm_vec;
                        for (auto & e : norm_vec)
                        {
                            is >> e;
                        }
                        vectors_norm.push_back(Normalize(norm_vec));
                    }
                    else if (prefix == "f ")
                    {
                        is.seekg(2);
                        Face f;
                        while (!is.eof())
                        {
                            FaceVtxAttrib vtx_attr = { 0, -1, -1 };
                            is >> vtx_attr.idx_v;
                            is.seekg(1, std::ios_base::cur);
                            is >> vtx_attr.idx_tex;
                            is.seekg(1, std::ios_base::cur);
                            is >> vtx_attr.idx_norm;
                            f.push_back(vtx_attr);
                        }
                        faces.push_back(std::move(f));
                    }
                }
            }
        }

    }

}
