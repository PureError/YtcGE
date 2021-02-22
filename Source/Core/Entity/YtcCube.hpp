#ifndef YTC_CUBE_HPP
#define YTC_CUBE_HPP

#include "YtcNode.hpp"

namespace YtcGE
{
    class Cube : public Node
    {
    public:
        enum VertexLocation
        {
            FAR_TOP_RIGHT = 0,
            FAR_TOP_LEFT,
            FAR_BOTTOM_LEFT,
            FAR_BOTTOM_RIGHT,
            NEAR_TOP_RIGHT,
            NEAR_TOP_LEFT,
            NEAR_BOTTOM_LEFT,
            NEAR_BOTTOM_RIGHT,
            
            VERTEX_COUNT
        };


        Cube(float length, float width, float height) noexcept;

        void TextureCoord(VertexLocation vl, float u, float v) noexcept
        {
            auto & texcoord = model_->vertices[vl]->texcoord;
            texcoord.U() = u;
            texcoord.V() = v;
        }

        void TextureCoord(const std::array<Vec2f, VERTEX_COUNT> & coords) noexcept
        {
            for (int i = 0; i < VERTEX_COUNT; ++i)
            {
                TextureCoord(VertexLocation(i), coords[i].U(), coords[i].V());
            }
        }

        virtual std::shared_ptr<Node> Clone() override;
    private:
        Vec3f lwh_;
    };
}

#endif