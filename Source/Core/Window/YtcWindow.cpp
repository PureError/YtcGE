#include "YtcWindow.hpp"
#include "Core/Utility/YtcException.hpp"

YtcGE::BasicWindow::BasicWindow(const Attributes& na)
    : attr_(na)
{
    WNDCLASSEX wc = {};
    wc.cbSize = sizeof(wc);
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = na.hInstance ? na.hInstance : GetModuleHandle(NULL);
    wc.lpszClassName = na.title.c_str();
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

    eventDispatcher_ = MakeUnique<EventDispatcher<UINT>>();
    auto hwnd = ::CreateWindowEx(0, wc.lpszClassName, Title().c_str(), WS_OVERLAPPEDWINDOW,
                                 CW_USEDEFAULT, CW_USEDEFAULT, Width(), Height(),
                                 NULL, NULL, wc.hInstance, NULL);

    if (hwnd == NULL)
    {
        throw Exception(_T("Failed to create window!"));
    }
    ::SetWindowLongPtr(hwnd, GWLP_USERDATA, LONG_PTR(this));
    wnd_ = hwnd;
    ::ShowWindow(wnd_, Visible() ? SW_SHOW : SW_HIDE);
}


YtcGE::BasicWindow::~BasicWindow()
{

}

void YtcGE::BasicWindow::Update()
{

}

void YtcGE::BasicWindow::FullScreen()
{

}

void YtcGE::BasicWindow::Close()
{
    ::PostMessage(wnd_, WM_QUIT, 0, 0);
}

void YtcGE::BasicWindow::DoResize(int w, int h)
{

}

void YtcGE::BasicWindow::SetVSync(bool enabled)
{
    if (enabled)
    {
        attr_.status |= VSYNC;
    }
    else
    {
        attr_.status &= (~VSYNC);
    }
}


void YtcGE::BasicWindow::SetVisiblity(bool visible)
{
    if (visible)
    {
        ::ShowWindow(wnd_, SW_SHOW);
        attr_.status |= VISIBLE;
    }
    else
    {
        ::ShowWindow(wnd_, SW_HIDE);
        attr_.status &= (~VISIBLE);
    }
}

LRESULT CALLBACK YtcGE::BasicWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    auto winPtr = reinterpret_cast<BasicWindow*>(::GetWindowLongPtr(hwnd, GWLP_USERDATA));
    if (winPtr)
    {
        return winPtr->OnWinMessage(uMsg, wParam, lParam);
    }
    return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT YtcGE::BasicWindow::OnWinMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_PAINT:
            break;
        case WM_SIZE:
            break;
        case WM_CLOSE:
            Close();
            break;
        case WM_DESTROY:
            break;
        default:
            break;
    }
    return ::DefWindowProc(wnd_, uMsg, wParam, lParam);
}

