#include "YtcWindow.hpp"
#include "Core/Utility/YtcException.hpp"

namespace YtcGE
{
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
        renderable_buffer_ = CreateRenderableBuffer();
    }


    YtcGE::BasicWindow::~BasicWindow()
    {
        if (bitmap_)
        {
            ::DeleteObject(bitmap_);
        }

        ::CloseWindow(Handle());
    }

    void YtcGE::BasicWindow::Update()
    {
#ifdef YTC_OS_WINDOWS
        HDC hdc = ::GetDC(Handle());
        ::BitBlt(hdc, 0, 0, Width(), Height(), mem_dc_, 0, 0, SRCCOPY);
        ::ReleaseDC(Handle(), hdc);
#endif
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

    LRESULT CALLBACK YtcGE::BasicWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept
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
                return TRUE;
                break;
            case WM_DESTROY:
                break;
            case WM_KEYDOWN:
            case WM_KEYUP:
                eventDispatcher_->Dispatch(uMsg, wParam);
                break;
            default:
                break;
        }
        return ::DefWindowProc(wnd_, uMsg, wParam, lParam);
    }

    std::unique_ptr<YtcGE::RenderableBuffer<uint32_t>> YtcGE::BasicWindow::CreateRenderableBuffer()
    {
        auto client_rect = ClientRect();
        int w = client_rect.Width();
        int h = client_rect.Height();
        uint32_t size = w * h * sizeof(uint32_t);
#ifdef YTC_OS_WINDOWS
        HDC hdc = ::GetDC(wnd_);
        auto mem_dc = ::CreateCompatibleDC(hdc);
        BITMAPINFO bitmap_info =
        {
            {
                sizeof(BITMAPINFOHEADER),
                w,
                0 - h,
                1,
                (sizeof(uint32_t) << 3),
                BI_RGB,
                size
            }
        };
        mem_dc_ = mem_dc;
        LPVOID buffer = nullptr;
        bitmap_ = ::CreateDIBSection(mem_dc, &bitmap_info, DIB_RGB_COLORS, &buffer, NULL, 0);
        ::ZeroMemory(buffer, size);
        auto ptr = MakeUnique<RenderableBuffer<uint32_t>>(buffer, w, h);
        ::SelectObject(mem_dc, bitmap_);
        return ptr;
#endif
    }

    YtcGE::Rect<int> BasicWindow::ClientRect() const noexcept
    {
        Rect<int> r;
#ifdef YTC_OS_WINDOWS
        ::GetClientRect(wnd_, reinterpret_cast<LPRECT>(&r));
#endif
        return r;
    }

}