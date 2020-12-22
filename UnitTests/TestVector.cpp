
#include "TestVector.hpp"
#include <sstream>
#include <iostream>
#include <cassert>

using namespace YtcGE;
using namespace std;

inline std::string ToString(const Vec4f& v)
{
    std::ostringstream ss;
    ss << "[";
    for (auto e : v)
    {
        ss << e << ",";
    }
    ss << "]";
    return ss.str();
}

static void TestCtor()
{
    Vec4f v0;
    Vec4f v1 = { 1,2,3,4 };
    Vec4f v2(Vec3f{ 1, 1, 1 });
    v0 = { 4, 5, 6, 7 };
    cout << ToString(v0) << endl;
    cout << ToString(v1) << endl;
    cout << ToString(v2) << endl;
}

static void TestOperations()
{
    Vec4f v{};
    Vec4f lhs(1.0f);
    Vec4f rhs(2.0f);
    auto sum = lhs + rhs;
    Vec4f expectedSum(3.0f);
    assert(sum.NearlyEqualTo(expectedSum));
    auto diff = rhs - lhs;
    Vec4f expectedDiff(1.0f);
    assert(expectedDiff.NearlyEqualTo(diff));
    constexpr float expectedDotProduct = 8.0f;
    assert(NearlyEqual(Dot(lhs, rhs), expectedDotProduct));
    Vec4f expectedScale4(4.0f);
    assert((lhs * 4.0f).NearlyEqualTo(expectedScale4));
    assert((rhs += lhs).NearlyEqualTo(expectedSum));
    assert((rhs *= 2.0f).NearlyEqualTo(Vec4f(6.0f)));
    assert((rhs /= 3.0f).NearlyEqualTo(Vec4f(2.0f)));
    assert((lhs -= Vec4f(1.0f)).NearlyEqualTo(Vec4f::Zero()));
    auto len = Length(Vec4f(1.0f));
    assert(NearlyEqual(len, 2.0f));
}


void YtcGE::DoTestVector()
{
    TestCtor();
    TestOperations();
}
