#ifndef YTC_WINDOW_HPP
#define YTC_WINDOW_HPP
#include "../Fundation.hpp"
#include "../StringUtils/YtcString.hpp"
#include "../Utility/YtcEventDispatcher.hpp"
#include <functional>
#include <bitset>
#ifdef YTC_OS_WINDOWS
#include <Windows.h>
#endif


namespace YtcGE
{
    class BasicWindow 
    {
    public:

        enum Status
        {
            VISIBLE = 1,
            ACTIVE = 1 << 1,
            VSYNC = 1 << 2,
        };
        
        struct Attributes
        {
            String title{ _T("YtcGE")};
            int width { 800 };
            int height { 600 };
            uint32_t status { VISIBLE | ACTIVE };
#ifdef YTC_OS_WINDOWS
            UINT style{};
            HINSTANCE hInstance{};
            HICON hIcon{};
            HCURSOR hCoursor{};
            HBRUSH hbrBackground{};
            HICON hIconSm{};
#endif
        };

        BasicWindow(const Attributes& na = Attributes());
        virtual ~BasicWindow();

        int Height() const
        {
            return attr_.height;
        }

        int Width() const
        {
            return attr_.width;
        }

        const String & Title() const
        {
            return attr_.title;
        }

        bool Visible() const
        {
            return (attr_.status & VISIBLE) == VISIBLE;
        }

        bool VSync() const
        {
            return (attr_.status & VSYNC) == VSYNC ;
        }

        bool Active() const
        {
            return (attr_.status & ACTIVE) == ACTIVE;
        }

        template<typename F>
        void AddEventListener(uint32_t e, F f)
        {
            eventDispatcher_->Register(std::move(f), e);
        }

        void SetVisiblity(bool visible);
        void SetVSync(bool sync);
        virtual void Update();
        virtual void FullScreen();
        virtual void Close();

#ifdef YTC_OS_WINDOWS
    public:
        HWND Handle() const
        {
            return wnd_;
        }
    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
        LRESULT OnWinMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        HWND wnd_;
#endif
    private:
        virtual void DoResize(int w, int h);
    private:
        UniquePtr<EventDispatcher<UINT>> eventDispatcher_;
        Attributes attr_;
    };

    using WindowPtr = SharedPtr<BasicWindow>;

}
#endif