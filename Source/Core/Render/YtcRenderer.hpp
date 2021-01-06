#ifndef YTC_RENDERER_HPP
#define YTC_RENDERER_HPP

#include "../Math/YtcPoint.hpp"
#include "YtcColor.hpp"
namespace YtcGE
{
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
    private:
        Renderer();
    };
}

#endif