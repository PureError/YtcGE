#ifndef YTC_INPUT_DEVICE_HPP
#define YTC_INPUT_DEVICE_HPP
#include "../StringUtils/YtcString.hpp"
namespace YtcGE
{
    namespace Input
    {
        namespace Device
        {
            enum class Category : uint8_t
            {
                KEYBOARD,
                MOUSE,

                COUNT,
            };
            constexpr uint32_t CategoryCount = static_cast<uint32_t>(Category::COUNT);

            class IDevice
            {
            public:
                virtual Device::Category Type() const noexcept = 0;
                virtual String ToString() const noexcept = 0;
                virtual ~IDevice() {}
            };
        }



        using DevicePtr = SharedPtr<Device::IDevice>;
    }
    
}



#endif