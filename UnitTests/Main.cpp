
#include "TestVector.hpp"
#include "TestString.hpp"
#include "TestMatrix.hpp"

#include <sstream>
#include <iostream>
#include <cassert>
using namespace YtcGE;
using namespace std;



int main()
{
    DoTestVector();
    DoTestString();
    DoTestMatrix();
    cin.get();
    return 0;
}