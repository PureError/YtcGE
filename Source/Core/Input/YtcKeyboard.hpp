#ifndef YTC_KEYBOARD_HPP
#define YTC_KEYBOARD_HPP

#include "YtcInputDevice.hpp"
#ifdef YTC_OS_WINDOWS
#include <windows.h>
#endif
namespace YtcGE
{
    namespace Input
    {
        namespace Device
        {
            class Keyboard : public IDevice
            {
            public:
                using KeyCode = uint32_t;
                enum class Key : uint16_t
                {
                    Undefined,
                    Space,
                    Apostrophe,
                    Comma,
                    Minus,
                    Period,
                    Slash,
                    D0,
                    D1,
                    D2,
                    D3,
                    D4,
                    D5,
                    D6,
                    D7,
                    D8,
                    D9,
                    Semicolon,
                    Equal,
                    A,
                    B,
                    C,
                    D,
                    E,
                    F,
                    G,
                    H,
                    I,
                    J,
                    K,
                    L,
                    M,
                    N,
                    O,
                    P,
                    Q,
                    R,
                    S,
                    T,
                    U,
                    V,
                    W,
                    X,
                    Y,
                    Z,
                    LeftBracket,
                    Backslash,
                    RightBracket,
                    GraveAccent,
                    World1,
                    World2,
                    /* Function keys */
                    Escape,
                    Enter,
                    Tab,
                    Backspace,
                    Insert,
                    Delete,
                    Right,
                    Left,
                    Down,
                    Up,
                    PageUp,
                    PageDown,
                    Home,
                    End,
                    CapsLock,
                    ScrollLock,
                    NumLock,
                    PrintScreen,
                    Pause,
                    F1,
                    F2,
                    F3,
                    F4,
                    F5,
                    F6,
                    F7,
                    F8,
                    F9,
                    F10,
                    F11,
                    F12,
                    F13,
                    F14,
                    F15,
                    F16,
                    F17,
                    F18,
                    F19,
                    F20,
                    F21,
                    F22,
                    F23,
                    F24,
                    F25,
                    /* Keypad */
                    KP0,
                    KP1,
                    KP2,
                    KP3,
                    KP4,
                    KP5,
                    KP6,
                    KP7,
                    KP8,
                    KP9,
                    KPDecimal,
                    KPDivide,
                    KPMultiply,
                    KPSubtract,
                    KPAdd,
                    KPEnter,
                    KPEqual,
                    LeftShift,
                    LeftControl,
                    LeftAlt,
                    LeftSuper,
                    RightShift,
                    RightControl,
                    RightAlt,
                    RightSuper,
                    Menu,

                    Count,
                };

                enum class KeyState : uint8_t
                {
                    Up,
                    Down,
                };

                static Key TranslateFromCode(KeyCode code);

                static constexpr size_t KeyCount = static_cast<size_t>(Key::Count) - 1;

                Keyboard() noexcept
                {
                    keymap_.fill(KeyState::Up);
                }
                ~Keyboard() = default;

                KeyState StateOf(Key key) const
                {
                    return keymap_[static_cast<size_t>(key)];
                }

                bool IsKeyDown(Key key) const
                {
                    return StateOf(key) == KeyState::Down;
                }

                bool IsKeyUp(Key key) const
                {
                    return StateOf(key) == KeyState::Up;
                }

                Category Type() const noexcept override
                {
                    return Category::KEYBOARD;
                }

                String ToString() const noexcept override
                {
                    return _T("[Keyboard]");
                }

                void OnKeyDown(Key key)
                {
                    keymap_[static_cast<size_t>(key)] = KeyState::Down;
                }

                void OnKeyUp(Key key)
                {
                    keymap_[static_cast<size_t>(key)] = KeyState::Up;
                }

            private:
                using KeyMap = std::array<KeyState, KeyCount>;
                KeyMap keymap_;
            };

            using KeyboardPtr = SharedPtr<Keyboard>;
            inline Keyboard::Key Input::Device::Keyboard::TranslateFromCode(KeyCode code)
            {
#ifdef YTC_OS_WINDOWS
                static constexpr Key Keys[] =
                {
                    Key::Backspace,// starts from VK_BACK 0x08
                    Key::Tab, //0x09
                    Key::Undefined, Key::Undefined, //0x0A-0B
                    Key::Undefined, //0x0C
                    Key::Enter,//0x0D
                    Key::Undefined, Key::Undefined, //0x0E-0x0F
                    Key::Undefined, //VK_SHIFT 0x10
                    Key::Undefined, //VK_CONTROL 0x11
                    Key::Undefined, //VK_MENU 0x12
                    Key::Pause,//0x13
                    Key::CapsLock,//0x14
                    Key::Undefined, //VK_KANA  VK_HANGUEL VK_HANGUL 0x15
                    Key::Undefined, //VK_IME_ON 0x16
                    Key::Undefined, //VK_JUNJA 0x17
                    Key::Undefined, //VK_FINAL 0x18
                    Key::Undefined, //VK_HANJA VK_KANJI  0x19
                    Key::Undefined, //VK_IME_OFF 0x1A
                    Key::Escape, //0x1B
                    Key::Undefined, //VK_CONVERT 0x1C
                    Key::Undefined, //VK_NONCONVERT 0x1D
                    Key::Undefined, //VK_ACCEPT 0x1E
                    Key::Undefined, //VK_MODECHANGE 0x1F
                    Key::Space, //0x20
                    Key::PageUp, //0x21
                    Key::PageDown, //0x22
                    Key::End, //0x23
                    Key::Home, //0x24
                    Key::Left, //0x25
                    Key::Up, //0x26
                    Key::Right, //0x27
                    Key::Down, //0x28
                    Key::Undefined, //VK_SELECT 0x29
                    Key::Undefined, //VK_PRINT 0x2A
                    Key::Undefined, //VK_EXECUTE 0x2B
                    Key::PrintScreen, //0x2C
                    Key::Insert, //0x2D
                    Key::Delete, //0x2E
                    Key::Undefined, //VK_HELP 0x2F
                    Key::D0, //0x30
                    Key::D1, //0x31
                    Key::D2, //0x32
                    Key::D3, //0x33
                    Key::D4, //0x34
                    Key::D5, //0x35
                    Key::D6, //0x36
                    Key::D7, //0x37
                    Key::D8, //0x38
                    Key::D9, //0x39
                    Key::Undefined, //0x3A
                    Key::Undefined, //0x3B
                    Key::Undefined, //0x3C
                    Key::Undefined, //0x3D 
                    Key::Undefined, //0x3E
                    Key::Undefined, //0x3F
                    Key::Undefined, //0x40
                    Key::A, //0x41
                    Key::B, //0x42
                    Key::C, //0x43
                    Key::D, //0x44
                    Key::E, //0x45
                    Key::F, //0x46
                    Key::G, //0x47
                    Key::H, //0x48
                    Key::I, //0x49
                    Key::J, //0x4A
                    Key::K, //0x4B
                    Key::L, //0x4C
                    Key::M, //0x4D
                    Key::N, //0x4E
                    Key::O, //0x4F
                    Key::P, //0x50
                    Key::Q, //0x51
                    Key::R, //0x52
                    Key::S, //0x53
                    Key::T, //0x54
                    Key::U, //0x55
                    Key::V, //0x56
                    Key::W, //0x57
                    Key::X, //0x58
                    Key::Y, //0x59
                    Key::Z, //0x5A
                    Key::Undefined, //VK_LWIN 0x5B
                    Key::Undefined, //VK_RWIN 0x5C
                    Key::Undefined, //VK_APPS 0x5D
                    Key::Undefined, //0x5E
                    Key::Undefined, //VK_SLEEP 0x5F
                    Key::KP0, //0x60
                    Key::KP1, //0x61
                    Key::KP2, //0x62
                    Key::KP3, //0x63
                    Key::KP4, //0x64
                    Key::KP5, //0x65
                    Key::KP6, //0x66
                    Key::KP7, //0x67
                    Key::KP8, //0x68
                    Key::KP9, //0x69
                    Key::KPMultiply, //0x6A
                    Key::KPAdd, //0x6B
                    Key::Undefined,//VK_SEPARATOR 0x6C
                    Key::KPSubtract, //0x6D
                    Key::KPDecimal, //0x6E
                    Key::KPDivide, //0x6F
                    Key::F1, //0x70
                    Key::F2, //0x71
                    Key::F3, //0x72
                    Key::F4, //0x73
                    Key::F5, //0x74
                    Key::F6, //0x75
                    Key::F7, //0x76
                    Key::F8, //0x77
                    Key::F9, //0x78
                    Key::F10,//0x79
                    Key::F11,//0x7A
                    Key::F12,//0x7B
                    Key::F13,//0x7C
                    Key::F14,//0x7D
                    Key::F15,//0x7E
                    Key::F16,//0x7F
                    Key::F17,//0x80
                    Key::F18,//0x81
                    Key::F19,//0x82
                    Key::F20,//0x83
                    Key::F21,//0x84
                    Key::F22,//0x85
                    Key::F23,//0x86
                    Key::F24,//0x87
                    Key::Undefined, //0x88
                    Key::Undefined, //0x89
                    Key::Undefined, //0x8A
                    Key::Undefined, //0x8B
                    Key::Undefined, //0x8C
                    Key::Undefined, //0x8D
                    Key::Undefined, //0x8E
                    Key::Undefined, //0x8F
                    Key::NumLock, //0x90
                    Key::ScrollLock, //0x91
                    Key::Undefined, //OEM specific 0x92
                    Key::Undefined, //OEM specific 0x93
                    Key::Undefined, //OEM specific 0x94
                    Key::Undefined, //OEM specific 0x95
                    Key::Undefined, //OEM specific 0x96
                    Key::Undefined, //Unassigned 0x97
                    Key::Undefined, //Unassigned 0x98
                    Key::Undefined, //Unassigned 0x99
                    Key::Undefined, //Unassigned 0x9A
                    Key::Undefined, //Unassigned 0x9B
                    Key::Undefined, //Unassigned 0x9C
                    Key::Undefined, //Unassigned 0x9D
                    Key::Undefined, //Unassigned 0x9E
                    Key::Undefined, //Unassigned 0x9F
                    Key::LeftShift, //Unassigned 0xA0
                    Key::RightShift,//Unassigned 0xA1
                    Key::LeftControl, //Unassigned 0xA2
                    Key::RightControl, //Unassigned 0xA3
                    Key::Undefined, //VK_LMENU 0xA4
                    Key::Undefined, //VK_RMENU 0xA5
                    Key::Undefined, //VK_BROWSER_BACK 0xA6
                    Key::Undefined, //VK_BROWSER_FORWARD 0xA7
                    Key::Undefined, //VK_BROWSER_REFRESH 0xA8
                    Key::Undefined, //VK_BROWSER_STOP 0xA9
                    Key::Undefined, //VK_BROWSER_SEARCH 0xAA
                    Key::Undefined, //VK_BROWSER_FAVORITES 0xAB
                    Key::Undefined, //VK_BROWSER_HOME 0xAC
                    Key::Undefined, //VK_VOLUME_MUTE 0xAD
                    Key::Undefined, //VK_VOLUME_DOWN 0xAE
                    Key::Undefined, //VK_VOLUME_UP 0xAF
                    Key::Undefined, //VK_MEDIA_NEXT_TRACK 0xB0
                    Key::Undefined, //VK_MEDIA_PREV_TRACK 0xB1
                    Key::Undefined, //VK_MEDIA_STOP 0xB2
                    Key::Undefined, //VK_MEDIA_PLAY_PAUSE 0xB3
                    Key::Undefined, //VK_LAUNCH_MAIL 0xB4
                    Key::Undefined, //VK_LAUNCH_MEDIA_SELECT 0xB5
                    Key::Undefined, //VK_LAUNCH_APP1 0xB6
                    Key::Undefined, //VK_LAUNCH_APP2 0xB7
                    Key::Undefined, Key::Undefined,//Reserved 0xB8-B9
                    Key::Undefined, //VK_OEM_1 0xBA
                    Key::Undefined, //VK_OEM_PLUS 0xBB
                    Key::Undefined, //VK_OEM_COMMA 0xBC
                    Key::Undefined, //VK_OEM_MINUS 0xBD
                    Key::Undefined, //VK_OEM_PERIOD 0xBE
                    Key::Undefined, //VK_OEM_2 0xBF
                    Key::Undefined, //VK_OEM_3 0xC0
                    Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined,
                    Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined,
                    Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined,
                    Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, //Reserved
                    Key::Undefined, Key::Undefined, Key::Undefined, //Unassigned
                    Key::Undefined, //VK_OEM_4 0xDB
                    Key::Undefined, //VK_OEM_5 0xDC
                    Key::Undefined, //VK_OEM_6 0xDD
                    Key::Undefined, //VK_OEM_7 0xDE
                    Key::Undefined, //VK_OEM_8 0xDF
                    Key::Undefined, //reserved 0xE0
                    Key::Undefined, //OEM specific 0xE1
                    Key::Undefined, //VK_OEM_102 0xE2
                    Key::Undefined, Key::Undefined, //OEM specific 0xE3-E4
                    Key::Undefined, //VK_PROCESSKEY 0xE5
                    Key::Undefined, //OEM specific 0xE6
                    Key::Undefined, //VK_PACKET 0xE7
                    Key::Undefined, //Unassigned 0xE8
                    Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined,
                    Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, Key::Undefined, //OEM specific
                    Key::Undefined, //VK_ATTN 0xF6
                    Key::Undefined, //VK_CRSEL 0xF7
                    Key::Undefined, //VK_EXSEL 0xF8
                    Key::Undefined, //VK_EREOF 0xF9
                    Key::Undefined, //VK_PLAY 0xFA
                    Key::Undefined, //VK_ZOOM 0xFB
                    Key::Undefined, //VK_NONAME 0xFC
                    Key::Undefined, //VK_PA1 0xFD
                    Key::Undefined, //VK_OEM_CLEAR 0xFE
                };
                return Keys[code - VK_BACK];
#endif
            }

        }
    }
}


#endif