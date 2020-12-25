#include "YtcWindowsWindow.hpp"
#include "Core/Utility/YtcException.hpp"

YtcGE::WindowsWindow::WindowsWindow(const CommonWindowAttributes& cwa, const NativeAttributes& na)
    : BaseWindow(cwa)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = na.hInstance ? na.hInstance : GetModuleHandle(NULL);
    wc.lpszClassName = cwa.title.c_str();
    wc.hIcon = na.hIcon;
    wc.hCursor = na.hCoursor;
    wc.hbrBackground = na.hbrBackground;
    wc.hIconSm = na.hIconSm;
    auto atom = ::RegisterClassEx(&wc);
    if (!atom)
    {
        auto err = ::GetLastError();
        StringStream ss; 
        ss << _T("Failed to register win-class, errcode:") << err; 
        throw Exception(ss.str());
    }

    auto hwnd = ::CreateWindowEx(0, wc.lpszClassName, Title().c_str(), WS_OVERLAPPEDWINDOW, 
                                 CW_USEDEFAULT, CW_USEDEFAULT, Width(), Height(), 
                                 NULL, NULL, wc.hInstance, NULL);

    if (hwnd == NULL)
    {
        throw Exception(_T("Failed to create window!"));
    }
    ::SetWindowLongPtr(hwnd, GWLP_USERDATA, LONG_PTR(this));
    wnd_ = hwnd;
    SetVisiblityImpl(cwa.visible);
}


YtcGE::WindowsWindow::~WindowsWindow()
{

}

void YtcGE::WindowsWindow::Update()
{

}

void YtcGE::WindowsWindow::FullScreen()
{

}

void YtcGE::WindowsWindow::Run()
{
    UniquePtr<MSG> msg = MakeUnique<MSG>();
    auto msgPtr = msg.get();
    auto hwnd = wnd_;
    BOOL hasMsg = ::PeekMessage(msgPtr, hwnd, 0, 0, PM_NOREMOVE);
    while (msgPtr->message != WM_QUIT)
    {
        if (Active())
        {
            hasMsg =::PeekMessage(msgPtr, hwnd, 0, 0, PM_REMOVE);
        }
        else
        {
            hasMsg = ::GetMessage(msgPtr, hwnd, 0, 0);
        }

        if (hasMsg)
        {
            ::TranslateMessage(msgPtr);
            ::DispatchMessage(msgPtr);
        }
        else
        {
            //‰÷»æ
        }
    }
}

void YtcGE::WindowsWindow::DoResize(int w, int h)
{

}

void YtcGE::WindowsWindow::SetVSyncImpl(bool enabled)
{

}


void YtcGE::WindowsWindow::SetVisiblityImpl(bool visible)
{
    ::ShowWindow(wnd_, visible ? SW_SHOW : SW_HIDE);
}

LRESULT CALLBACK YtcGE::WindowsWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto winPtr = reinterpret_cast<WindowsWindow*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (winPtr)
    {
        return winPtr->OnWinMessage(uMsg, wParam, lParam);
    }
    return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT YtcGE::WindowsWindow::OnWinMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    }
    return ::DefWindowProc(wnd_, uMsg, wParam, lParam);
}

