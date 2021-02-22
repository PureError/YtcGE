#include "YtcCube.hpp"
namespace YtcGE
{

    Cube::Cube(float length, float width, float height) noexcept
        : lwh_{ length, width, height }
    {
        Vec3f lwh_half = lwh_ / 2;
        this->model_ = MakeShared<Model>();
        auto& vertices = model_->vertices;
        vertices.resize(VERTEX_COUNT);
        std::for_each(vertices.begin(), vertices.end(), [](VertexAttribPtr& ptr) 
        {
            ptr = MakeShared<VertexAttrib>();
        });
        vertices[FAR_TOP_RIGHT]->position = { lwh_half.X(), lwh_half.Y(), lwh_half.Z(), 1.0f };
        vertices[FAR_TOP_LEFT]->position = { -lwh_half.X(), lwh_half.Y(), lwh_half.Z(), 1.0f };
        vertices[FAR_BOTTOM_LEFT]->position = { -lwh_half.X(), -lwh_half.Y(), lwh_half.Z(), 1.0f };
        vertices[FAR_BOTTOM_RIGHT]->position = { lwh_half.X(), -lwh_half.Y(), lwh_half.Z(), 1.0f };
        vertices[NEAR_TOP_RIGHT]->position = { lwh_half.X(), lwh_half.Y(), -lwh_half.Z(), 1.0f };
        vertices[NEAR_TOP_LEFT]->position = { -lwh_half.X(), lwh_half.Y(), -lwh_half.Z(), 1.0f };
        vertices[NEAR_BOTTOM_LEFT]->position = { -lwh_half.X(), -lwh_half.Y(), -lwh_half.Z(), 1.0f };
        vertices[NEAR_BOTTOM_RIGHT]->position = { lwh_half.X(), -lwh_half.Y(), -lwh_half.Z(), 1.0f };

        model_->indices =
        {
            { 0, 1, 2 }, //far
            { 0, 2, 3 },
            { 4, 5, 6 }, //near
            { 4, 6, 7 },
            { 0, 1, 5 }, //top
            { 0, 4, 5 },
            { 2, 3, 6 }, //bottom
            { 3, 6, 7 },
            { 1, 2, 5 }, //left
            { 2, 5, 6 },
            { 0, 3, 4 }, //right
            { 3, 4, 7 },
        };

    }

    std::shared_ptr<YtcGE::Node> Cube::Clone()
    {
        return std::static_pointer_cast<Node>(std::make_shared<Cube>(lwh_.X(), lwh_.Y(), lwh_.Z()));
    }

}