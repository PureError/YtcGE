#ifndef YTC_WINDOW_HPP
#define YTC_WINDOW_HPP
#include "../Fundation.hpp"
#include "../StringUtils/YtcString.hpp"
#include <functional>
namespace YtcGE
{
    struct CommonWindowAttributes
    {
        String title{"YtcGE"};
        int width{800};
        int height{600};
        bool vsync = false;
        bool active = false;
        bool visible = true;
    };
    
    class BaseWindow
    {
    public:
        using EventHanlder = std::function<void(BaseWindow&)>;
        BaseWindow(const CommonWindowAttributes& attr);
        virtual ~BaseWindow();
        virtual void Update() = 0;
        virtual void FullScreen() = 0;
        virtual void Run() = 0;
        int Width() const noexcept 
        {
            return attr_.width; 
        }

        int Height() const noexcept
        {
            return attr_.height;
        }
        
        const String& Title() const noexcept
        {
            return attr_.title;
        }

        bool Active() const noexcept
        {
            return attr_.active;
        }

        EventHanlder& OnPaint() noexcept
        {
            return onPaint_;
        }

        EventHanlder& OnResize() noexcept
        {
            return onResize_;
        }

        EventHanlder& OnClosed() noexcept
        {
            return onClosed_;
        }

        EventHanlder& OnActive() noexcept
        {
            return onActive_;
        }

        void SetVisible(bool visible)
        {
            SetVisiblityImpl(visible);
            attr_.visible = visible;
        }

        bool Visible() const noexcept
        {
            return attr_.visible;
        }

        void SetVSync(bool enabled) noexcept
        {
            attr_.vsync = enabled;
            SetVSyncImpl(enabled);
        }

        bool VSync() const noexcept
        {
            return attr_.vsync;
        }

        void Resize(int w, int h)
        {
            DoResize(w, h);
            attr_.width = w;
            attr_.height = h;
        }

    private:
        virtual void DoResize(int w, int h) = 0;
        virtual void SetVSyncImpl(bool enabled) = 0;
        virtual void SetVisiblityImpl(bool visible) = 0;
    private:
        CommonWindowAttributes attr_;
        EventHanlder onPaint_;
        EventHanlder onResize_;
        EventHanlder onClosed_;
        EventHanlder onActive_;
    };

    using WindowPtr = SharedPtr<BaseWindow>;
}

#endif