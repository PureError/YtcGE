
#include "TestVector.hpp"
#include "TestString.hpp"
#include "TestMatrix.hpp"
#include <core/Utility/YtcEventDispatcher.hpp>
#ifdef YTC_OS_WINDOWS
#include <Core/Window/YtcWindowsWindow.hpp>
#endif

#include <sstream>
#include <iostream>
#include <cassert>
using namespace YtcGE;
using namespace std;


int main()
{
    CommonWindowAttributes attr{};
    WindowPtr win = MakeShared<WindowsWindow>(attr);
    win->Run();

    return 0;
}