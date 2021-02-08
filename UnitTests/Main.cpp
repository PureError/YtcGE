//#include "TestVector.hpp"
//#include "TestString.hpp"
//#include "TestMatrix.hpp"
//#include "TestSingleton.hpp"
#include <core/Utility/YtcEventDispatcher.hpp>


#include <Core/App/YtcApplication.hpp>
#include <Core/Render/YtcVertex.hpp>
#include <Core/Render/YtcImage.hpp>
#include <sstream>
#include <iostream>
#include <cassert>

namespace YtcGE
{
    UniquePtr<YtcGE::Application> App;
}

using namespace YtcGE;
using namespace std;


int main(int argc, const Char* argv[])
{
    std::vector<String> cmdArgs(argc);
    std::copy(argv, argv + argc, cmdArgs.begin());
    App = Application::Create(cmdArgs);
    App->Run();
    return 0;
}