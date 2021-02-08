#ifndef YTC_INPUT_MANAGER_HPP
#define YTC_INPUT_MANAGER_HPP

#include "../Utility/YtcSingleton.hpp"
#include "../Input/YtcInput.hpp"

namespace YtcGE
{
    class InputManager : public Singleton<InputManager>
    {
        MARK_AS_SINGLETON();
        InputManager() noexcept;
        ~InputManager() = default;
    public:
        using DeviceList = std::vector<Input::DevicePtr>;
        Input::Device::KeyboardPtr Keyboard() noexcept
        {
            return CastPtr<Input::Device::Keyboard>(UniqueDevice(Input::Device::Category::KEYBOARD));
        }

        void OnKeyUp(Input::Device::Keyboard::KeyCode keycode);
        void OnKeyDown(Input::Device::Keyboard::KeyCode keycode);
        Input::DevicePtr UniqueDevice(Input::Device::Category type) noexcept;
    private:
        DeviceList dev_list_unique_;
        DeviceList dev_list_multi_;
    };
}

#endif