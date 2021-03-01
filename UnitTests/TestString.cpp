#include "TestString.hpp"
#include <Core/Log/YtcLog.hpp>
#include <cassert>
#include <iostream>
using namespace std;
using namespace YtcGE;

namespace UnitTest
{

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


    static void TestFormat()
    {
        std::cout << String::Format(_T("ytc $fff ${$} {1} {0} {2} old£¡\n"), 18, _T("is"), _T("years"));
        std::cout << String::Format(_T("my hair length is {0} cm\n"), 5.2f);
        std::cout << String::Format(_T("insufficient arguments {0}, {1}, {2}"), String(_T("[only one]")));
    }

    void DoTestString()
    {
        YTC_LOG("Start string test:\n");
        //TestTrim();
        TestFormat();

        std::cin.get();
    }
}