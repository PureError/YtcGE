#ifndef YTC_MOUSE_HPP
#define YTC_MOUSE_HPP
#include "YtcInputDevice.hpp"

namespace YtcGE
{
    namespace Input
    {
        namespace Device
        {
            class Mouse : public IDevice
            {
            public:
                Category Type() const noexcept override
                {
                    return Category::MOUSE;
                }
            };
        }
    }
}


#endif