#include "YtcRenderer.hpp"
#include "../App/YtcApplication.hpp"
#include "../Log/YtcLog.hpp"
#include "../Utility/Localization.hpp"
#ifdef YTC_OS_WINDOWS
#include <windows.h>
#endif
extern YtcGE::UniquePtr<YtcGE::Application> App;

namespace YtcGE
{
    YtcGE::Renderer::Renderer()
    {

    }


    void Renderer::BackGround(const ColorF & color) noexcept
    {
        App->MainWindow()->RenderBuffer().FillWith(color.ToUint32());
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



    void Renderer::DrawTriangle(const std::array<Vertex*, 3> & vertices, const Texture2D::Ptr & texture)
    {
        std::array<YtcGE::Trapzoid, 2> traps;
        int count_traps = SplitTriangle(vertices, traps);
        auto & render_buffer = App->MainWindow()->RenderBuffer();
        if (texture)
        {
			for (int i = 0; i < count_traps; ++i)
			{
				auto& trap = traps[i];
				auto& e0_v0 = *trap.edges[0][0];
				auto& e0_v1 = *trap.edges[0][1];
				auto& e1_v0 = *trap.edges[1][0];
				auto& e1_v1 = *trap.edges[1][1];

				float h_0 = e0_v1.position.Y() - e0_v0.position.Y();
				float h_1 = e1_v1.position.Y() - e1_v0.position.Y();
				int top = static_cast<int>(trap.top + 0.5f);
				int bottom = static_cast<int>(trap.bottom + 0.5f);
				for (int pos_y = top; pos_y < bottom; ++pos_y)
				{
					float s0 = (static_cast<float>(pos_y) + 0.5f - e0_v0.position.Y()) / h_0;
					float s1 = (static_cast<float>(pos_y) + 0.5f - e1_v0.position.Y()) / h_1;
					auto vtx_0 = Lerp(e0_v0, e0_v1, s0);
					auto vtx_1 = Lerp(e1_v0, e1_v1, s1);

					int w = int(vtx_1.position.X() + 0.5f) - int(vtx_0.position.X() + 0.5f);
					int abs_w = Abs(w);
					auto stride_position = (vtx_1.position - vtx_0.position) / abs_w;
					auto stride_color = (vtx_1.color - vtx_0.color) / abs_w;
					auto stride_texcoord = (vtx_1.texcoord - vtx_0.texcoord) / abs_w;
					auto vtx = vtx_0;
					int total_steps = abs_w + 1;
					while (total_steps > 0)
					{
						auto clr_tex = texture->Sample(vtx.texcoord.U(), vtx.texcoord.V());
						uint32_t x = static_cast<uint32_t>(vtx.position.X() + 0.5f);
						uint32_t y = static_cast<uint32_t>(vtx.position.Y() + 0.5f);
						float z = vtx.position.Z();
						uint32_t color = (clr_tex.R() << 16) | (clr_tex.G() << 8) | (clr_tex.B() << 0) | (clr_tex.A() << 24);
						render_buffer.SetPixel(x, y, color, z);
						vtx.position += stride_position;
						//vtx_left.color += stride_color;
						vtx.texcoord += stride_texcoord;
						--total_steps;
					}
				}
			}

        }
        else
        {
			Point2i p0, p1, p2;
			p0.X() = static_cast<int>(vertices[0]->position.X());
			p0.Y() = static_cast<int>(vertices[0]->position.Y());
			p1.X() = static_cast<int>(vertices[1]->position.X());
			p1.Y() = static_cast<int>(vertices[1]->position.Y());
			p2.X() = static_cast<int>(vertices[2]->position.X());
			p2.Y() = static_cast<int>(vertices[2]->position.Y());
			static const ColorF White(255.0f, 255.0f, 255.0f, 255.0f);
			static const ColorF Black(0.0f, 0.0f, 0.0f, 0.0f);
			DrawLine2D(p0, p1, White);
			DrawLine2D(p0, p2, White);
			DrawLine2D(p1, p2, White);
        }

    }

    bool Renderer::DrawTriangle3D(const std::array<VertexPtr, 3> & vertices, const Texture2D::Ptr & texture)
    {
        std::array<Vertex, 3> vertices_proj;
        auto& render_buffer = App->MainWindow()->RenderBuffer();
        auto max_x = render_buffer.Width() - 1;
        auto max_y = render_buffer.Height() - 1;
        for (int i = 0; i < 3; ++i)
        {
             auto & vtx = *(vertices[i]);
             auto & v_proj = vertices_proj[i];
             v_proj.position = vtx.position * this->mat_;
             float w = v_proj.position.W();
             if (NearlyEqual(w, 0.0f))
             {
                 return false;
             }
             v_proj.position /= w;
             if ((Abs(v_proj.position.X()) >= 1.0f) || (Abs(v_proj.position.Y()) >= 1.0f))//YTC_RT_ASSERT((Abs(v_proj.position.X()) <= 1.0f) && (Abs(v_proj.position.Y()) <= 1.0f));
             {
                 return false;
             }
             float x = (v_proj.position.X() + 1.0f) * max_x * 0.5f + 0.5f;
             float y = (1.0f - v_proj.position.Y()) * max_y * 0.5f + 0.5f;
             v_proj.position.X() = x;
             v_proj.position.Y() = y;
             v_proj.color = vtx.color;
             v_proj.texcoord = vtx.texcoord;
        }
        std::array<Vertex*, 3> vtx_ptr_proj;
        std::transform(vertices_proj.begin(), vertices_proj.end(), vtx_ptr_proj.begin(), [](Vertex& v) { return &v; });
        DrawTriangle(vtx_ptr_proj, texture);
        return true;
    }

    void Renderer::DrawModel(const Model& model)
    {
        std::array<VertexPtr, 3> triangle;
        auto& indices = model.indices;
        auto& vertices = model.vertices;
        auto& texture = model.texture;
        for (auto & i : indices)
        {
            for (size_t j = 0; j < 3; ++j)
            {
                triangle[j] = vertices[i[j]];
            }
            DrawTriangle3D(triangle, texture);
        }
    }

    int Renderer::SplitTriangle(std::array<Vertex*, 3> t, std::array<YtcGE::Trapzoid, 2> & trapzoids) noexcept
    {
        std::sort(t.begin(), t.end(), [](const Vertex* lhs, const Vertex* rhs)
        {
            return lhs->position.Y() < rhs->position.Y();
        });
        const auto & v0 = t[0];
        const auto & v1 = t[1];
        const auto & v2 = t[2];
        int count = 0;
        if (Abs(v0->position.Y() - v1->position.Y()) > 0.001f)//if (!NearlyEqual(v0->position.Y(), v1->position.Y()))
        {
            auto & trap = trapzoids[count];
            trap.top = v0->position.Y();
            trap.bottom = v1->position.Y();
            trap.edges[0][0] = v0;
            trap.edges[0][1] = v1;
            trap.edges[1][0] = v0;
            trap.edges[1][1] = v2;
            ++count;
        }

        if (Abs(v1->position.Y() - v2->position.Y()) > 0.001f)//if (!NearlyEqual(v1->position.Y(), v2->position.Y()))
        {
            auto & trap = trapzoids[count];
            trap.top = v1->position.Y();
            trap.bottom = v2->position.Y();
            trap.edges[0][0] = v1;
            trap.edges[0][1] = v2;
            trap.edges[1][0] = v0;
            trap.edges[1][1] = v2;
            ++count;
        }

        return count;
    }


    void Renderer::DrawLine2D(const Point2i & from, const Point2i & to, const ColorF & color) noexcept
    {
        auto& render_buffer = App->MainWindow()->RenderBuffer();
        auto dx = to.X() - from.X();
        auto dy = to.Y() - from.Y();
        auto rgba = color.ToUint32();
        if (Abs(dx) < Abs(dy))
        {
            int step_y = to.Y() > from.Y() ? 1 : -1;
            if (dx == 0) // vertical line 
            {
                for (int y = from.Y(), x = from.X(); y != to.Y(); y += step_y)
                {
                    render_buffer.SetPixel(x, y, rgba);
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
                    render_buffer.SetPixel(x, y, rgba);

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
                    render_buffer.SetPixel(x, y, rgba);

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
                    render_buffer.SetPixel(x, y, rgba);

                }
            }
        }
        render_buffer.SetPixel(to.X(), to.Y(), rgba);
    }

}
