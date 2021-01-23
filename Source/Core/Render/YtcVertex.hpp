#ifndef YTC_VERTEX_HPP
#define YTC_VERTEX_HPP

#include "../Math/YtcPoint.hpp"
#include "YtcColor.hpp"
#include "../Utility/Localization.hpp"
namespace YtcGE
{


    struct Vertex
    {
        Point4f position;
        ColorF color;
        Vec2f texcoord;
    };


    template<>
    inline Vertex Lerp<Vertex>(const Vertex & v1, const Vertex & v2, float s) noexcept
    {
        return
        {
            Lerp(v1.position, v2.position, s),
            Lerp(v1.color, v2.color, s),
            Lerp(v1.texcoord, v2.texcoord, s),
        };
    }
    using VertexPtr = std::shared_ptr<Vertex>;
}


#endif