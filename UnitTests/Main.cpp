
#include "TestVector.hpp"
#include "TestString.hpp"
#include "TestMatrix.hpp"
#include <core/Utility/YtcEventDispatcher.hpp>


#include <Core/App/YtcApplication.hpp>
#include <Core/Render/YtcVertex.hpp>
#include <Core/Render/YtcImage.hpp>
#include <sstream>
#include <iostream>
#include <cassert>
using namespace YtcGE;
using namespace std;


UniquePtr<Application> App;
int main(int argc, const Char* argv[])
{
    auto img = Image::FromFile("Snipaste_2021-01-09_10-23-03.bmp");
    std::vector<String> cmdArgs(argc);
    std::copy(argv, argv + argc, cmdArgs.begin());
    App = Application::Create(cmdArgs);
    App->Run();
    return 0;
}