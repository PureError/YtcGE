#ifndef YTC_POINT_HPP
#define YTC_POINT_HPP

#include "YtcVector.hpp"
namespace YtcGE
{
    
    template<typename T, int N>
    using Point_T = Vector<T, N>;
    using Point2f = Point_T<float, 2>;
    using Point2i = Point_T<int, 2>;
    using Point3f = Point_T<float, 3>;
    using Point3i = Point_T<int, 3>;
    using Point4i = Point_T<int, 4>;
    using Point4f = Point_T<float, 4>;
}

#endif