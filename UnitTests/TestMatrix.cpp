#include "TestMatrix.hpp"

#include <Core/Fundation.hpp>
#include <iostream>
using namespace YtcGE;
using namespace std;


template<typename T, int R, int C>
inline ostream& operator<<(ostream& stream, const Matrix<T, R, C>& m) noexcept
{
    for (auto& vec : m)
    {
        stream << "\n[";
        for (auto e : vec)
        {
            stream << e << ",";
        }
        stream << "]\n";
    }
    return stream;
}

void DoTestMatrix()
{
    Mat44f m = {};
    Mat44f m1 = 
    {
        1.1f, 1.0f, 1.0f, 1.0f,
        2.0f, 2.0f, 2.0f, 2.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        2.0f, 2.0f, 2.0f, 2.0f,
    };

    Mat44f m2 =
    {
        { 1.1f, 1.0f, 1.0f, 1.0f },
        { 2.0f, 2.0f, 2.0f, 2.0f },
        { 1.0f, 1.0f, 1.0f, 1.0f },
        { 2.0f, 2.0f, 2.0f, 2.0f },
    };

    Mat44f m1plusm2 = 
    {
        2.2f, 2.0f, 2.0f, 2.0f,
        4.0f, 4.0f, 4.0f, 4.0f,
        2.0f, 2.0f, 2.0f, 2.0f,
        4.0f, 4.0f, 4.0f, 4.0f,
    };

    assert((m1 + m2).NearlyEqualTo(m1plusm2));
    assert((m1 * 2).NearlyEqualTo(m1plusm2));
    assert((m1plusm2 / 2).NearlyEqualTo(m1));
    assert((m1 - m2).NearlyEqualTo(Mat44f::Zero()));
    assert((m1 * SquareMatrixHelper44f::Identity()).NearlyEqualTo(m1));
    assert(m1 != m1plusm2);
    assert((m1plusm2 /= 2).NearlyEqualTo(m1));
    assert(m1 == m2);
    m1 = SquareMatrixHelper44f::Identity();

    Matrix<int, 2, 3> m23(2);
    Matrix<int, 3, 4> m34(1);
    Matrix<int, 2, 4> expectedM(6);
    assert((m23 * m34) == expectedM);

    Vec4f v(1.0f);
    assert(v.NearlyEqualTo(v * SquareMatrixHelper44f::Identity()));

    Mat44i mi(m1);
    cout << m1 << m2 << mi;
}
