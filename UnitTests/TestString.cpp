#include "TestString.hpp"
#include <Core/Log/YtcLog.hpp>
#include <cassert>
using namespace std;
using namespace YtcGE;


static void TestTrim()
{
    static String inputs[] =
    {
        _T("    hello  "),
        _T("    hello\n  "),
        _T("  \thello\n  "),
        _T(" \t"),
        _T(""),
        _T("xxx"),
    };

    static String expectedOutputs[] =
    {
        _T("hello"),
        _T("hello"),
        _T("hello"),
        _T(""),
        _T(""),
        _T("xxx"),
    };

    for (int i = 0; i < sizeof(inputs) / sizeof(*inputs); ++i)
    {
        auto actualOutput = Trim(inputs[i]);
        assert(actualOutput == expectedOutputs[i]);
    }
}

void DoTestString()
{
    YTC_LOG("Start string test:");
    TestTrim();
}
