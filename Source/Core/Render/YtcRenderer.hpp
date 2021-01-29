#ifndef YTC_RENDERER_HPP
#define YTC_RENDERER_HPP

#include "YtcModel.hpp"
namespace YtcGE
{

    struct Trapzoid
    {
        float top;
        float bottom;
        std::array<std::array<Vertex*, 2>, 2> edges;
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
        void DrawLine(const Point3f & from, const Point3f & to, const ColorF & color) noexcept;
        void DrawPoint(const Point3f & point, const ColorF & color) noexcept;
        void DrawLine2D(const Point2i & from, const Point2i & to, const ColorF & color)noexcept;
        void DrawPoint(const Point2i & coordOnScreen, const ColorF & color) noexcept;
        void DrawTriangle(const std::array<Vertex*, 3> & vertices, const Texture2D::Ptr & texture);
        bool DrawTriangle3D(const std::array<VertexPtr, 3> & vertices, const Texture2D::Ptr & texture);
        void DrawModel(const Model& model);

        void SetTransformMatrix(const Mat44f& m) noexcept
        {
            this->mat_ = m;
        }
    private:
        int SplitTriangle(std::array<Vertex*, 3> t, std::array<Trapzoid, 2> & traps) noexcept;
        Renderer();
        Mat44f mat_ = SquareMatrixHelper44f::Identity();
    };
}

#endif