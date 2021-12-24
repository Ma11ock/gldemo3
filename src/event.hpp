#ifndef CHAR_EVENT_HPP
#define CHAR_EVENT_HPP

#include "util.hpp"

#include <cstdint>
#include <type_traits>

namespace proj
{
    /// Char's representation of keycodes.
    enum class KeyCode : std::uint32_t
    {
        Unknown = 0,

        Return = '\r',
        Escape = '\033',
        Backspace = '\b',
        Tab = '\t',
        Space = ' ',
        Exclaim = '!',
        Quotedbl = '"',
        Hash = '#',
        Percent = '%',
        Dollar = '$',
        Ampersand = '&',
        Quote = '\'',
        Leftparen = '(',
        Rightparen = ')',
        Asterisk = '*',
        Plus = '+',
        Comma = ',',
        Minus = '-',
        Period = '.',
        Slash = '/',
        K0 = '0',
        K1 = '1',
        K2 = '2',
        K3 = '3',
        K4 = '4',
        K5 = '5',
        K6 = '6',
        K7 = '7',
        K8 = '8',
        K9 = '9',

        Colon = ':',
        Semicolon = ';',
        Less = '<',
        Equals = '=',
        Greater = '>',
        Question = '?',
        At = '@',
        Leftbracket = '[',
        Backslash = '\\',
        Rightbracket = ']',
        Caret = '^',
        Underscore = '_',
        Backquote = '`',
        A = 'a',
        B = 'b',
        C = 'c',
        D = 'd',
        E = 'e',
        F = 'f',
        G = 'g',
        H = 'h',
        I = 'i',
        J = 'j',
        K = 'k',
        L = 'l',
        M = 'm',
        N = 'n',
        O = 'o',
        P = 'p',
        Q = 'q',
        R = 'r',
        S = 's',
        T = 't',
        U = 'u',
        V = 'v',
        W = 'w',
        X = 'x',
        Y = 'y',
        Z = 'z',
        Delete = '\177',

        Capslock,

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

        Printscreen,
        Scrolllock,
        Pause,
        Insert,
        Home,
        Pageup,
        End,
        Pagedown,
        Right,
        Left,
        Down,
        Up,

        Numlockclear,
        KP_Divide,
        KP_Multiply,
        KP_Minus,
        KP_Plus,
        KP_Enter,
        KP_1,
        KP_2,
        KP_3,
        KP_4,
        KP_5,
        KP_6,
        KP_7,
        KP_8,
        KP_9,
        KP_0,
        KP_Period,

        Application,
        Power,
        KP_Equals,
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
        Execute,
        Help,
        Menu,
        Select,
        Stop,
        Again,
        Undo,
        Cut,
        Copy,
        Paste,
        Find,
        Mute,
        Volumeup,
        Volumedown,
        KP_Comma,
        KP_Equalsas400,

        Alterase,
        Sysreq,
        Cancel,
        Clear,
        Prior,
        Return2,
        Separator,
        Out,
        Oper,
        Clearagain,
        Crsel,
        Exsel,

        KP_00,
        KP_000,
        Thousandsseparator,
        Decimalseparator,
        Currencyunit,
        Currencysubunit,
        KP_Leftparen,
        KP_Rightparen,
        KP_Leftbrace,
        KP_Rightbrace,
        KP_Tab,
        KP_Backspace,
        KP_A,
        KP_B,
        KP_C,
        KP_D,
        KP_E,
        KP_F,
        KP_Xor,
        KP_Power,
        KP_Percent,
        KP_Less,
        KP_Greater,
        KP_Ampersand,
        KP_Dblampersand,
        KP_Verticalbar,
        KP_Dblverticalbar,
        KP_Colon,
        KP_Hash,
        KP_Space,
        KP_At,
        KP_Exclam,
        KP_Memstore,
        KP_Memrecall,
        KP_Memclear,
        KP_Memadd,
        KP_Memsubtract,
        KP_Memmultiply,
        KP_Memdivide,
        KP_Plusminus,
        KP_Clear,
        KP_Clearentry,
        KP_Binary,
        KP_Octal,
        KP_Decimal,
        KP_Hexadecimal,

        LCtrl,
        LShift,
        LAlt,
        LGui,
        RCtrl,
        RShift,
        RAlt,
        RGUI,

        Mode,

        Audionext,
        Audioprev,
        Audiostop,
        Audioplay,
        Audiomute,
        Mediaselect,
        WWW,
        Mail,
        Calculator,
        Computer,
        AC_Search,
        AC_Home,
        AC_Back,
        AC_Forward,
        AC_Stop,
        AC_Refresh,
        AC_Bookmarks,

        Brightnessdown,
        Brightnessup,
        Displayswitch,
        Kbdillumtoggle,
        Kbdillumdown,
        Kbdillumup,
        Eject,
        Sleep,
        App1,
        App2,

        Audiorewind,
        Audiofastforward,
    };

    /**
     * @brief Checks if a keycode has an ASCII representation.
     *
     * @param kc They keycode to check.
     * @return True if the keycode has an ASCII character, false if not.
     */
    bool isCharKey(std::uint32_t code);

    /// Enum representing modifier keys.
    enum class KeyModifiers : std::uint16_t
    {
        /// No modifier keys.
        None = 0,
        LShift = proj::Bit16(0),
        RShift = proj::Bit16(1),
        LCtrl = proj::Bit16(2),
        RCtrl = proj::Bit16(3),
        LAlt = proj::Bit16(4),
        RAlt = proj::Bit16(5),
        LSuper = proj::Bit16(6),
        RSuper = proj::Bit16(7),
        Numlock = proj::Bit16(8),
        Caps = proj::Bit16(9),
        Mode = proj::Bit16(10),
        Ctrl = proj::Bit16(11) | LCtrl | RCtrl,
        Shift = proj::Bit16(12) | LShift | RShift,
        Alt = proj::Bit16(13) | LAlt | RAlt,
        Super = proj::Bit16(14) | LSuper | RSuper,
    };

    enum class EventType : std::uint32_t
    {
        None = 0,

        KeyPressed = proj::Bit(0),
        KeyHold = KeyPressed | proj::Bit(1),
        KeyReleased = proj::Bit(2),
        KeyTextInput = proj::Bit(3) | KeyPressed,

        MousePressed = proj::Bit(4),
        MouseReleased = proj::Bit(5),
        MouseMoved = proj::Bit(6),
        MouseScrolled = proj::Bit(7),

        WindowClose = proj::Bit(8),
        WindowResize = proj::Bit(9),
        WindowFocus = proj::Bit(10),
        WindowLoseFocus = proj::Bit(11),
        WindowMoved = proj::Bit(12),
    };

    enum class EventCategory : std::uint32_t
    {
        None = 0,
        Input = proj::Bit(0),
        KeyInput = proj::Bit(1) | Input,
        MouseInput = proj::Bit(2) | Input,
        MouseButton = proj::Bit(3) | MouseInput,
    };

    class Event
    {
    public:
        virtual ~Event() = default;

        virtual EventCategory getCategory() const
        { return _category; }

        virtual EventType getEventType() const
        { return _type; }

        virtual bool isHandled() const
        { return _handled; }

        virtual void setHandled(bool handled)
        { _handled = handled; }
    protected:
        Event()
            : _handled(false),_category(EventCategory::None),
              _type(EventType::None)
        {
        }

        Event(EventCategory category, EventType type)
            : _handled(false),_category(category),_type(type)
        {
        }

        bool _handled;
        EventCategory _category;
        EventType _type;
    };
}

ENABLE_BITMASK_OPERATORS(proj::EventCategory)
ENABLE_BITMASK_OPERATORS(proj::EventType)
ENABLE_BITMASK_OPERATORS(proj::KeyModifiers)

#endif /* CHAR_EVENT_HPP */
