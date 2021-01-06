#include "YtcRenderer.hpp"
#include "../App/YtcApplication.hpp"

#ifdef YTC_OS_WINDOWS
#include <windows.h>
#endif
extern YtcGE::UniquePtr<YtcGE::Application> App;

namespace YtcGE
{
    YtcGE::Renderer::Renderer()
    {

    }


    void Renderer::DrawLine(const Point3f & from, const Point3f & to, const ColorF & color) noexcept
    {

    }

    void Renderer::DrawPoint(const Point3f & point, const ColorF & color) noexcept
    {

    }

    void Renderer::DrawPoint(const Point2i & coordOnScreen, const ColorF & color) noexcept
    {
        auto rgba = color.RGBA8888();
#ifdef YTC_OS_WINDOWS
        COLORREF rgb = RGB(std::get<0>(rgba), std::get<1>(rgba), std::get<2>(rgba));
        HWND hwnd = App->MainWindow()->Handle();
        HDC hdc = ::GetDC(hwnd);
        ::SetPixel(hdc, coordOnScreen.X(), coordOnScreen.Y(), rgb);
       ::ReleaseDC(hwnd, hdc);
#endif
    }

    void Renderer::DrawTriangle2D(std::array<Point2i, 3> points, const ColorF& color, bool filling) noexcept
    {
        std::sort(points.begin(), points.end(), [](const Point2i & lhs, const Point2i & rhs)
        {
            return lhs.Y() < rhs.Y();
        });
        const auto & a = points[0];
        const auto & b = points[1];
        const auto & c = points[2];

        if (filling)
        {
            int step_y = 1;
            int hab = b.Y() - a.Y();
            int hac = c.Y() - a.Y();
            int steps = 0;
            for (auto y = a.Y(); y <= b.Y(); y = a.Y() + (steps += step_y))
            {
                float s = float(steps) / hab;
                auto p1 = Lerp(a, b, s);
                s = float(steps) / hac;
                auto p2 = Lerp(a, c, s);
                p1.Y() = p2.Y() = y;
                DrawLine2D(p1, p2, color);
            }
            steps = 0;
            step_y = -1;
            int hcb = b.Y() - c.Y();
            hac = a.Y() - c.Y();
            for (auto y = c.Y(); y != b.Y(); y = c.Y() + (steps += step_y))
            {
                float s = float(steps) / hcb;
                auto p1 = Lerp(c, b, s);
                s = float(steps) / hac;
                auto p2 = Lerp(c, a, s);
                p1.Y() = p2.Y() = y;
                DrawLine2D(p1, p2, color);
            }
        }

        DrawLine2D(a, b, color);
        DrawLine2D(a, c, color);
        DrawLine2D(b, c, color);
    }

    void Renderer::DrawLine2D(const Point2i & from, const Point2i & to, const ColorF & color) noexcept
    {
        HWND hwnd = App->MainWindow()->Handle();
        HDC hdc = ::GetDC(hwnd);
        auto dx = to.X() - from.X();
        auto dy = to.Y() - from.Y();
        auto rgba = color.RGBA8888();
        COLORREF rgb = RGB(std::get<0>(rgba), std::get<1>(rgba), std::get<2>(rgba));
        if (Abs(dx) < Abs(dy))
        {
            int step_y = to.Y() > from.Y() ? 1 : -1;
            if (dx == 0) // vertical line 
            {
                for (int y = from.Y(), x = from.X(); y != to.Y(); y += step_y)
                {
                    ::SetPixel(hdc, x, y, rgb);
                }
            }
            else
            {
                float k = 1.0f * dx / dy;
                float step_x = k * step_y;
                for (int y = from.Y(), x = from.X(), step_count = 0; y != to.Y(); ++step_count)
                {
                    y += step_y;
                    x = from.X() + step_count * step_x;
                    ::SetPixel(hdc, x, y, rgb);
                }
            }
        }
        else
        {
            int step_x = to.X() > from.X() ? 1 : -1;
            if (dy == 0) // horizontal line
            {
                for (int x = from.X(), y = from.Y(); x != to.X(); x += step_x)
                {
                    ::SetPixel(hdc, x, y, rgb);
                }
            }
            else
            {
                float k = 1.0f * dy / dx;
                float step_y = step_x * k;
                for (int x = from.X(), y = from.Y(), step_count = 0; x != to.X(); ++step_count)
                {
                    x += step_x;
                    y = from.Y() + step_count * step_y;
                    ::SetPixel(hdc, x, y, rgb);
                }
            }
        }
        ::SetPixel(hdc, to.X(), to.Y(), rgb);
        ::ReleaseDC(hwnd, hdc);
    }

}
