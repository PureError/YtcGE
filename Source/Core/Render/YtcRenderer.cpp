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

    void Renderer::DrawTriangle(const Triangle & t) noexcept
    {
        auto traps = SplitTriangle(t);

        for (auto & trap : traps)
        {
            int h = static_cast<int>(trap.bottom - trap.top + 0.5f);
            auto & left_v0 = *trap.edge_left[0];
            auto & left_v1 = *trap.edge_left[1];
            auto & right_v0 = *trap.edge_right[0];
            auto & right_v1 = *trap.edge_right[1];

            float h_left = left_v1.position.Y() - left_v0.position.Y();
            float h_right = right_v1.position.Y() - right_v0.position.Y();

            for (int y_steps = 0; y_steps <= h; ++y_steps)
            {
                auto vtx_left = Lerp(left_v0, left_v1, (float)y_steps / h_left);
                auto vtx_right = Lerp(right_v0, right_v1, (float)y_steps / h_right);

                auto w = vtx_right.position.X() - vtx_left.position.X();
                
            }
        }
    }


    std::array<YtcGE::Trapzoid, 2> Renderer::SplitTriangle(const Triangle & t) noexcept
    {
        std::sort(t.vertices.begin(), t.vertices.end(), [](const Vertex & lhs, const Vertex & rhs)
        {
            return lhs.position.Y() < rhs.position.Y();
        });
        const auto & v0 = t.vertices[0];
        const auto & v1 = t.vertices[1];
        const auto & v2 = t.vertices[2];
        std::array<YtcGE::Trapzoid, 2> trapzoids;
        auto & t1 = trapzoids[0];
        auto & t2 = trapzoids[1];
        t1.top = v0->position.Y();
        t1.bottom = v1->position.Y();
        t1.edge_left[0] = v0;
        t1.edge_left[1] = v1;
        t2.edge_right[0] = v0;
        t2.edge_right[1] = v2;

        t2.top = v1->position.Y();
        t2.bottom = v2->position.Y();
        t2.edge_left[0] = v1;
        t2.edge_left[1] = v2;
        t2.edge_right[0] = v0;
        t2.edge_right[1] = v2;

        return trapzoids;
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
                    x = from.X() + step_count * step_x + 0.5f;
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
                    y = from.Y() + step_count * step_y + 0.5f;
                    ::SetPixel(hdc, x, y, rgb);
                }
            }
        }
        ::SetPixel(hdc, to.X(), to.Y(), rgb);
        ::ReleaseDC(hwnd, hdc);
    }

}
