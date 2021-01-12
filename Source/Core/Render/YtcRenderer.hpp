#ifndef YTC_RENDERER_HPP
#define YTC_RENDERER_HPP

#include "YtcVertex.hpp"
#include "YtcTexture.hpp"
namespace YtcGE
{

    struct Triangle
    {
        std::array<VertexPtr, 3> vertices;
        Texture2D::Ptr texture;
    };

    struct Trapzoid
    {
        float top;
        float bottom;
        std::array<std::array<VertexPtr, 2>, 2> edges;
    };

    class Renderer
    {
    public:
        static Renderer & Get()
        {
            static Renderer instance;
            return instance;
        }
        void DrawLine(const Point3f & from, const Point3f & to, const ColorF & color) noexcept;
        void DrawPoint(const Point3f & point, const ColorF & color) noexcept;
        void DrawTriangle2D(std::array<Point2i, 3> points, const ColorF& color, bool filling = true) noexcept;
        void DrawLine2D(const Point2i & from, const Point2i & to, const ColorF & color)noexcept;
        void DrawPoint(const Point2i & coordOnScreen, const ColorF & color) noexcept;
        void DrawTriangle(const Triangle & t) noexcept;
    private:
        std::array<Trapzoid, 2> SplitTriangle(Triangle t) noexcept;
        Renderer();
    };
}

#endif