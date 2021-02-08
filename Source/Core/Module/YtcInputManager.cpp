#include "YtcInputManager.hpp"
#include "../Utility/YtcException.hpp"
#include "../App/YtcApplication.hpp"
#include "../Utility/Localization.hpp"
namespace YtcGE
{
    using namespace Input;
    InputManager::InputManager() noexcept
    {
        dev_list_unique_.push_back(std::static_pointer_cast<Device::IDevice>(MakeShared<Device::Keyboard>()));
        auto window =  App->MainWindow();
#ifdef YTC_OS_WINDOWS
        window->AddEventListener(WM_KEYUP, ToStdFunction([this](Input::Device::Keyboard::KeyCode keycode) { OnKeyUp(keycode); }) );
        window->AddEventListener(WM_KEYDOWN, ToStdFunction([this](Input::Device::Keyboard::KeyCode keycode) { OnKeyDown(keycode); }));
#endif
    }


    void InputManager::OnKeyUp(Input::Device::Keyboard::KeyCode keycode)
    {
        auto dev = Keyboard();
        if (dev)
        {
            auto key = Device::Keyboard::TranslateFromCode(keycode);
            CastPtr<Device::Keyboard>(dev)->OnKeyUp(key);
        }
    }

    void InputManager::OnKeyDown(Input::Device::Keyboard::KeyCode keycode)
    {
        auto dev = Keyboard();
        if (dev)
        {
            auto key = Device::Keyboard::TranslateFromCode(keycode);
            CastPtr<Device::Keyboard>(dev)->OnKeyDown(key);
        }
    }

    YtcGE::Input::DevicePtr InputManager::UniqueDevice(Input::Device::Category type) noexcept
    {
        auto iter = std::find_if(dev_list_unique_.begin(), dev_list_unique_.end(), [=](DeviceList::const_reference dev)
        {
            return dev->Type() == type;
        });

        return iter == dev_list_unique_.end() ? nullptr : (*iter);
    }

}

