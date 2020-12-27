#include "YtcApplication.hpp"

#ifdef YTC_OS_WINDOWS
#include "../Window/YtcWindowsWindow.hpp"
#endif

#include <thread>

struct YtcGE::Application::Impl
{
	std::thread winThread;
	WindowPtr win;
};

YtcGE::Application::Application(const std::vector<String>& cmdArgs) : cmds_(cmdArgs), impl_(new Impl())
{

}

YtcGE::Application::Application() : impl_(new Impl())
{

}

void YtcGE::Application::Run()
{
	impl_->winThread = std::thread([this]()
	{
		impl_->win = MakeWindow();
		impl_->win->Run();
	});

}

void YtcGE::Application::Terminate()
{
	if (impl_->winThread.joinable())
	{
		impl_->winThread.join();
	}
}

YtcGE::Application::~Application()
{
	Terminate();
}

YtcGE::WindowPtr YtcGE::Application::MakeWindow() const
{
	return MakeShared<WindowsWindow>(CommonWindowAttributes());
}
