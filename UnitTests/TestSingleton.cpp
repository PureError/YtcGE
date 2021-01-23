#include "TestSingleton.hpp"
#include <iostream>

using namespace YtcGE;

class Foo : public Singleton<Foo>
{
    MARK_AS_SINGLETON();
public:
    int Value() const noexcept
    {
        return value_;
    }
private:
    Foo() = default;
    ~Foo() noexcept
    {
        std::cout << "Foo ctor!\n";
    }
    int value_ = {};
};

class Bar : public Singleton<Bar, std::mutex>
{
    MARK_AS_SINGLETON();
public:
    int Value() const noexcept
    {
        return value_;
    }
private:
    Bar() = default;
    ~Bar() noexcept
    {
        std::cout << "Bar ctor!\n";
    }
    int value_ = {};
};

void DoTestSingleton()
{
    Foo::Instance().Value();
    Bar::Instance().Value();
}
