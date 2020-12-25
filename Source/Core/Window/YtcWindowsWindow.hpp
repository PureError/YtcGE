#ifndef YTC_WINDOWS_WINDOW_HPP
#define YTC_WINDOWS_WINDOW_HPP


#include "YtcWindow.hpp"
#include <windows.h>
namespace YtcGE
{
    class WindowsWindow : public BaseWindow
    {
    public:
        struct NativeAttributes
        {
            UINT style{};
            HINSTANCE hInstance{};
            HICON hIcon{};
            HCURSOR hCoursor{};
            HBRUSH hbrBackground{};
            HICON hIconSm{};
        };
        WindowsWindow(const CommonWindowAttributes& cwa, const NativeAttributes& na = NativeAttributes());
        virtual ~WindowsWindow();

        virtual void Update() override;
        virtual void FullScreen() override;
        virtual void Run() override;
    private:
        virtual void DoResize(int w, int h) override;
        virtual void SetVSyncImpl(bool enabled) override;
        virtual void SetVisiblityImpl(bool visible) override;
    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        HWND wnd_;
    };
}
#endif