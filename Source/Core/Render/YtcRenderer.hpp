#ifndef YTC_RENDERER_HPP
#define YTC_RENDERER_HPP

#include "YtcModel.hpp"
#include "../Utility/YtcEventHandler.hpp"
namespace YtcGE
{

    struct Trapzoid
    {
        using Edge = std::array<VertexAttrib*, 2>;
        float top;
        float bottom;
        std::array<Edge, 2> edges;
    };

    class Renderer
    {
    public:
        static Renderer & Get()
        {
            static Renderer instance;
            return instance;
        }
        void BackGround(const ColorF & color) noexcept;
        void DrawLine2D(const Point2i & from, const Point2i & to, const ColorF & color)noexcept;
        void DrawPoint(const Point2u point, const Color32b& color, float z) noexcept;
        void DrawTriangle(const std::array<VertexAttrib*, 3> & vertices, const Texture2D::Ptr & texture);
        bool DrawTriangle3D(const std::array<VertexAttribPtr, 3> & vertices, const Texture2D::Ptr & texture);
        void DrawModel(const Model& model);
        void SetTransformMatrix(const Mat44f& m) noexcept { this->mat_ = m; }
    private:
        int SplitTriangle(std::array<VertexAttrib*, 3> t, std::array<Trapzoid, 2> & traps) noexcept;
        Renderer();
        Mat44f mat_ = SquareMatrixHelper44f::Identity();
    };
}

#endif