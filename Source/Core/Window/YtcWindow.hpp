#ifndef YTC_WINDOW_HPP
#define YTC_WINDOW_HPP
#include "../Fundation.hpp"
#include "../StringUtils/YtcString.hpp"
#include "../Utility/YtcEventDispatcher.hpp"
#include "../Render/YtcRenderableBuffer.hpp"
#include "../Math/YtcRect.hpp"
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
            String title{ _T("YtcGameEngine")};
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

        int Height() const noexcept
        {
            return attr_.height;
        }

        int Width() const noexcept
        {
            return attr_.width;
        }

        const String & Title() const noexcept
        {
            return attr_.title;
        }

        bool Visible() const noexcept
        {
            return (attr_.status & VISIBLE) == VISIBLE;
        }

        bool VSync() const noexcept
        {
            return (attr_.status & VSYNC) == VSYNC ;
        }

        bool Active() const noexcept
        {
            return (attr_.status & ACTIVE) == ACTIVE;
        }

        Rect<int> ClientRect() const noexcept;

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

        RenderableBuffer<uint32_t> & RenderBuffer() noexcept
        {
            return *renderable_buffer_;
        }
#ifdef YTC_OS_WINDOWS
    public:
        HWND Handle() const noexcept
        {
            return wnd_;
        }

    private:
        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) noexcept;
        LRESULT OnWinMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
        HWND wnd_;
        HBITMAP bitmap_;
        HDC mem_dc_;
#endif
    private:
        virtual void DoResize(int w, int h);
        std::unique_ptr<RenderableBuffer<uint32_t>> CreateRenderableBuffer();
    private:
        UniquePtr<EventDispatcher<UINT>> eventDispatcher_;
        Attributes attr_;
        std::unique_ptr<RenderableBuffer<uint32_t>> renderable_buffer_;
    };

    using WindowPtr = SharedPtr<BasicWindow>;

}
#endif