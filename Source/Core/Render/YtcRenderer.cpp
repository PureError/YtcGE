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
        const auto * texture = t.texture.get();
        auto & render_buffer = App->MainWindow()->RenderBuffer();
        for (auto & trap : traps)
        {
            int h = static_cast<int>(trap.bottom - trap.top + 0.5f);
            auto & e0_v0 = *trap.edges[0][0];
            auto & e0_v1 = *trap.edges[0][1];
            auto & e1_v0 = *trap.edges[1][0];
            auto & e1_v1 = *trap.edges[1][1];

            float h_0 = e0_v1.position.Y() - e0_v0.position.Y();
            float h_1 = e1_v1.position.Y() - e1_v0.position.Y();

            for (int pos_y = static_cast<int>(trap.top + 0.5f); pos_y <= trap.bottom; ++pos_y)
            {
                auto vtx_0 = Lerp(e0_v0, e0_v1, (float)(pos_y - e0_v0.position.Y()) / h_0);
                auto vtx_1 = Lerp(e1_v0, e1_v1, (float)(pos_y - e1_v0.position.Y()) / h_1);

                int w = int(vtx_1.position.X() + 0.5f) - int(vtx_0.position.X() + 0.5f);
                int abs_w = Abs(w);
                auto stride_position = (vtx_1.position - vtx_0.position) / abs_w;
                auto stride_color = (vtx_1.color - vtx_0.color) / abs_w;
                auto stride_texcoord = (vtx_1.texcoord - vtx_0.texcoord) / abs_w;
                auto vtx = vtx_0;
                int total_steps = abs_w + 1;
                while(total_steps > 0)
                {
                    auto clr_tex = texture->Sample(vtx.texcoord.U(), vtx.texcoord.V());
                    uint32_t x = static_cast<uint32_t>(vtx.position.X() + 0.5f);
                    uint32_t y = static_cast<uint32_t>(vtx.position.Y() + 0.5f);
                    uint32_t color = (clr_tex.R() << 24) | (clr_tex.G() << 16) | (clr_tex.B() << 8) | clr_tex.A();
                    render_buffer.SetPixel(x, y, color);
                    vtx.position += stride_position;
                    //vtx_left.color += stride_color;
                    vtx.texcoord += stride_texcoord;
                    --total_steps;
                }
            }
        }

    }


    std::array<YtcGE::Trapzoid, 2> Renderer::SplitTriangle(Triangle t) noexcept
    {
        std::sort(t.vertices.begin(), t.vertices.end(), [](const VertexPtr & lhs, const VertexPtr & rhs)
        {
            return lhs->position.Y() < rhs->position.Y();
        });
        const auto & v0 = t.vertices[0];
        const auto & v1 = t.vertices[1];
        const auto & v2 = t.vertices[2];
        std::array<YtcGE::Trapzoid, 2> trapzoids;
        auto & t1 = trapzoids[0];
        auto & t2 = trapzoids[1];
        t1.top = v0->position.Y();
        t1.bottom = v1->position.Y();
        t1.edges[0][0] = v0;
        t1.edges[0][1] = v1;
        t1.edges[1][0] = v0;
        t1.edges[1][1] = v2;

        t2.top = v1->position.Y();
        t2.bottom = v2->position.Y();
        t2.edges[0][0] = v1;
        t2.edges[0][1] = v2;
        t2.edges[1][0] = v0;
        t2.edges[1][1] = v2;

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
