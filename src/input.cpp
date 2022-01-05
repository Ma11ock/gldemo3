#include "input.hpp"

#include <SDL.h>

#include <utility>
#include <unordered_map>
#include <type_traits>
#include <iostream>
#include <cstdint>
#include "event.hpp"
#include "frame.hpp"
#include "keyboardEvent.hpp"
#include "mouseEvent.hpp"
#include "util.hpp"

namespace
{
    // Mouse button states.
    std::array<bool, 6> mouseButtonIsDown{false}; 
    /* True if the user is trying to quit. False if not. */
    bool isQuit = false;
    /* The x position of the mouse. */
    float mouseX = 0.f;
    /* The y position of the mouse. */
    float mouseY = 0.f;
    /* Keystates. */
    std::unordered_map<SDL_Keycode,
                       std::pair<proj::EventType, SDL_Keymod>> keys;
    /* String that gets SDL's text input. */
    std::string textInput;
    /* Converts SDL keyboard input data to a KeyInput. */
    inline std::pair<proj::EventType, SDL_Keymod>
    createKeyModifier(SDL_KeyboardEvent &kbe)
    {
        proj::EventType ks = proj::EventType::None;
        switch(kbe.state)
        {
        case SDL_PRESSED:
            ks = kbe.repeat ? proj::EventType::KeyHold
                : proj::EventType::KeyPressed;
            break;
        case SDL_RELEASED:
            ks = proj::EventType::KeyReleased;
            break;
        default:
            break;
        }

        return std::make_pair(ks,
                              static_cast<SDL_Keymod>(kbe.keysym.mod));
    }
    /* Check to see if a key combination is pressed. */
    inline proj::EventType
    internalGetKeyState(SDL_Keycode code,
                        const std::vector<proj::KeyModifiers> &mods)
    {
        constexpr proj::EventType nothing = proj::EventType::None;
        auto [state, kmod] = keys[code];
        if(state == nothing)
            return nothing;
        for(const auto &mod : mods)
        {
            switch(mod)
            {
            case proj::KeyModifiers::None:
                return kmod == 0 ? state : nothing;
                break;
            case proj::KeyModifiers::LShift:
                state = kmod & KMOD_LSHIFT ? state : nothing;
                break;
            case proj::KeyModifiers::RShift:
                state = kmod & KMOD_RSHIFT ? state : nothing;
                break;
            case proj::KeyModifiers::LAlt:
                state = kmod & KMOD_LALT ? state : nothing;
                break;
            case proj::KeyModifiers::RAlt:
                state = kmod & KMOD_RALT ? state : nothing;
                break;
            case proj::KeyModifiers::LCtrl:
                state = kmod & KMOD_LCTRL ? state : nothing;
                break;
            case proj::KeyModifiers::RCtrl:
                state = kmod & KMOD_RCTRL ? state : nothing;
                break;
            case proj::KeyModifiers::LSuper:
                state = kmod & KMOD_LGUI ? state : nothing;
                break;
            case proj::KeyModifiers::RSuper:
                state = kmod & KMOD_RGUI ? state : nothing;
                break;
            case proj::KeyModifiers::Numlock:
                state = kmod & KMOD_NUM ? state : nothing;
                break;
            case proj::KeyModifiers::Caps:
                state = kmod & KMOD_CAPS ? state : nothing;
                break;
            case proj::KeyModifiers::Mode:
                state = kmod & KMOD_MODE ? state : nothing;
                break;
            case proj::KeyModifiers::Ctrl:
                state = (kmod & KMOD_LCTRL || kmod & KMOD_RCTRL) ? state : nothing;
                break;
            case proj::KeyModifiers::Shift:
                state = (kmod & KMOD_LSHIFT || kmod & KMOD_RSHIFT) ? state : nothing;
                break;
            case proj::KeyModifiers::Alt:
                state = (kmod & KMOD_LALT || kmod & KMOD_RALT)  ? state : nothing;
                break;
            case proj::KeyModifiers::Super:
                state = (kmod & KMOD_LGUI || kmod & KMOD_RGUI) ? state : nothing;
                break;
            default:
                break;
            }

            if(state == nothing)
                return nothing;
        }

        return state;
    }

    inline proj::MouseCode projMouseEventToCode(std::uint8_t code)
    {
        switch(code)
        {
        case SDL_BUTTON_LEFT:
            return proj::MouseCode::Left;
            break;
        case SDL_BUTTON_RIGHT:
            return proj::MouseCode::Right;
            break;
        case SDL_BUTTON_MIDDLE:
            return proj::MouseCode::Middle;
            break;
        case SDL_BUTTON_X1:
            return proj::MouseCode::X1;
            break;
        case SDL_BUTTON_X2:
            return proj::MouseCode::X2;
            break;
        default:
            break;
        }

        return proj::MouseCode::None;
    }

    inline void createKeyDownEvent(proj::KeyCode kc)
    {
        frame::addToEventQueue(std::make_shared<proj::KeyPressedEvent>(proj::EventType::KeyPressed, kc));
    }

    inline void createKeyHoldEvent(proj::KeyCode kc)
    {
        frame::addToEventQueue(std::make_shared<proj::KeyHoldEvent>(kc));
    }

    inline void createTextEvent()
    {
        frame::addToEventQueue(std::make_shared<proj::KeyTextInput>(::textInput));
    }

    inline void createKeyReleaseEvent(proj::KeyCode kc)
    {
        frame::addToEventQueue(std::make_shared<proj::KeyReleaseEvent>(kc));
    }

    inline void createMouseMotionEvent(float x, float y)
    {
        mouseX = x;
        mouseY = y;
        frame::addToEventQueue(std::make_shared<proj::MouseMove>(x, y));
    }

    inline void createMouseButtonEvent(proj::EventType type, proj::MouseCode code)
    {
        frame::addToEventQueue(std::make_shared<proj::MouseButton>(type, code));
    }

    inline void createMouseWheelEvent(std::int32_t x, std::int32_t y)
    {
        frame::addToEventQueue(std::make_shared<proj::MouseWheel>(x, y));
    }
}

static SDL_Keycode charKeyCodeToSDL(proj::KeyCode kc);
static proj::KeyCode sdlKeyCodeToChar(SDL_Keycode kc);

/* Poll the operating system for input. */
void input::pollInput()
{
    // Reset text input.
    textInput.clear();

    SDL_StartTextInput();
    for(SDL_Event event = {}; SDL_PollEvent(&event); )
    {
        switch(event.type)
        {
        case SDL_TEXTINPUT:
            textInput += event.text.text;
            createTextEvent();
            break;
        case SDL_QUIT:
            isQuit = true;
            break;
        case SDL_KEYDOWN:
        {
            auto sym = event.key.keysym.sym;
            auto tmpKeyInfo = createKeyModifier(event.key);
            keys[sym] = tmpKeyInfo;
            createKeyDownEvent(sdlKeyCodeToChar(sym));
        }
        break;
        case SDL_KEYUP:
        {
            auto sym = event.key.keysym.sym;
            auto tmpKeyInfo = createKeyModifier(event.key);
            keys[sym] = tmpKeyInfo;
            createKeyReleaseEvent(sdlKeyCodeToChar(sym));
        }
        break;
        case SDL_MOUSEMOTION:
        {
            createMouseMotionEvent(static_cast<float>(event.button.x),
                                   static_cast<float>(event.button.y));
        }
        break;
        case SDL_MOUSEBUTTONDOWN:
        {
            auto mouseButton = projMouseEventToCode(event.button.button);
            mouseButtonIsDown[static_cast<std::uint8_t>(mouseButton)] = true;
            createMouseButtonEvent(proj::EventType::MousePressed,
                                   mouseButton);
        }
        break;
        case SDL_MOUSEBUTTONUP:
        {
            auto mouseButton = projMouseEventToCode(event.button.button);
            mouseButtonIsDown[static_cast<std::uint8_t>(mouseButton)] = false;
            createMouseButtonEvent(proj::EventType::MouseReleased,
                                   mouseButton);
        }
        break;
        case SDL_MOUSEWHEEL:
        {
            createMouseWheelEvent(event.wheel.x, event.wheel.y);
        }
        break;
        default:
            break;
        }
    }
    for(std::size_t i = 0; i < mouseButtonIsDown.size(); i++)
        if(mouseButtonIsDown[i])
            createMouseButtonEvent(proj::EventType::MousePressed,
                                   static_cast<proj::MouseCode>(i)); 

    for(auto &[key, value] : keys)
        if(static_cast<bool>(value.first & proj::EventType::KeyPressed))
            createKeyHoldEvent(sdlKeyCodeToChar(key));
        else
            value = std::make_pair(proj::EventType::None, KMOD_NONE);

    SDL_StopTextInput();
}

/* Return true if the user is trying to quit. */
bool input::tryQuit()
{
    return isQuit;
}

/* Get the coordinates of the mouse (x,y). */
std::tuple<float, float> input::getMousePos()
{
    return std::make_tuple(::mouseX, ::mouseY);
}

/* Get the key state of the '0' key . */
proj::EventType input::getKeyState(char code, const std::vector<proj::KeyModifiers> &mods)
{
    return internalGetKeyState(static_cast<SDL_Keycode>(code), mods);
}

/* Get the key state of the '0' key . */
proj::EventType input::getKeyState(proj::KeyCode code,
                                   const std::vector<proj::KeyModifiers> &mods)
{
    return internalGetKeyState(charKeyCodeToSDL(code), mods);
}
/* Get a string of the last alphanumeric keypresses.
   Returns an empty string if no keypresses occurred. */
std::string input::getAlphaNumericKeyPresses()
{
    return textInput;
}


SDL_Keycode charKeyCodeToSDL(proj::KeyCode kc)
{
    /* If kc is ASCII, there's no need to convert. */
    if(proj::isCharKey(static_cast<std::uint32_t>(kc)))
        return static_cast<SDL_Keycode>(kc);
    /* Huge lookup table for converting a proj::KeyCode to an SDL_Keycode. */
    switch(kc)
    {
    case proj::KeyCode::Capslock:
        return SDLK_CAPSLOCK;
        break;

    case proj::KeyCode::F1:
        return SDLK_F1;
        break;
    case proj::KeyCode::F2:
        return SDLK_F2;
        break;
    case proj::KeyCode::F3:
        return SDLK_F3;
        break;
    case proj::KeyCode::F4:
        return SDLK_F4;
        break;
    case proj::KeyCode::F5:
        return SDLK_F5;
        break;
    case proj::KeyCode::F6:
        return SDLK_F6;
        break;
    case proj::KeyCode::F7:
        return SDLK_F7;
        break;
    case proj::KeyCode::F8:
        return SDLK_F8;
        break;
    case proj::KeyCode::F9:
        return SDLK_F9;
        break;
    case proj::KeyCode::F10:
        return SDLK_F10;
        break;
    case proj::KeyCode::F11:
        return SDLK_F11;
        break;
    case proj::KeyCode::F12:
        return SDLK_F12;
        break;

    case proj::KeyCode::Printscreen:
        return SDLK_PRINTSCREEN;
        break;
    case proj::KeyCode::Scrolllock:
        return SDLK_SCROLLLOCK;
        break;
    case proj::KeyCode::Pause:
        return SDLK_PAUSE;
        break;
    case proj::KeyCode::Insert:
        return SDLK_INSERT;
        break;
    case proj::KeyCode::Home:
        return SDLK_HOME;
        break;
    case proj::KeyCode::Pageup:
        return SDLK_PAGEUP;
        break;
    case proj::KeyCode::End:
        return SDLK_END;
        break;
    case proj::KeyCode::Pagedown:
        return SDLK_PAGEDOWN;
        break;
    case proj::KeyCode::Right:
        return SDLK_RIGHT;
        break;
    case proj::KeyCode::Left:
        return SDLK_LEFT;
        break;
    case proj::KeyCode::Down:
        return SDLK_DOWN;
        break;
    case proj::KeyCode::Up:
        return SDLK_UP;
        break;

    case proj::KeyCode::Numlockclear:
        return SDLK_NUMLOCKCLEAR;
        break;
    case proj::KeyCode::KP_Divide:
        return SDLK_KP_DIVIDE;
        break;
    case proj::KeyCode::KP_Multiply:
        return SDLK_KP_MULTIPLY;
        break;
    case proj::KeyCode::KP_Minus:
        return SDLK_KP_MINUS;
        break;
    case proj::KeyCode::KP_Plus:
        return SDLK_KP_PLUS;
        break;
    case proj::KeyCode::KP_Enter:
        return SDLK_KP_ENTER;
        break;
    case proj::KeyCode::KP_1:
        return SDLK_KP_1;
        break;
    case proj::KeyCode::KP_2:
        return SDLK_KP_2;
        break;
    case proj::KeyCode::KP_3:
        return SDLK_KP_3;
        break;
    case proj::KeyCode::KP_4:
        return SDLK_KP_4;
        break;
    case proj::KeyCode::KP_5:
        return SDLK_KP_5;
        break;
    case proj::KeyCode::KP_6:
        return SDLK_KP_6;
        break;
    case proj::KeyCode::KP_7:
        return SDLK_KP_7;
        break;
    case proj::KeyCode::KP_8:
        return SDLK_KP_8;
        break;
    case proj::KeyCode::KP_9:
        return SDLK_KP_9;
        break;
    case proj::KeyCode::KP_0:
        return SDLK_KP_0;
        break;
    case proj::KeyCode::KP_Period:
        return SDLK_KP_PERIOD;
        break;

    case proj::KeyCode::Application:
        return SDLK_APPLICATION;
        break;
    case proj::KeyCode::Power:
        return SDLK_POWER;
        break;
    case proj::KeyCode::KP_Equals:
        return SDLK_KP_EQUALS;
        break;
    case proj::KeyCode::F13:
        return SDLK_F13;
        break;
    case proj::KeyCode::F14:
        return SDLK_F14;
        break;
    case proj::KeyCode::F15:
        return SDLK_F15;
        break;
    case proj::KeyCode::F16:
        return SDLK_F16;
        break;
    case proj::KeyCode::F17:
        return SDLK_F17;
        break;
    case proj::KeyCode::F18:
        return SDLK_F18;
        break;
    case proj::KeyCode::F19:
        return SDLK_F19;
        break;
    case proj::KeyCode::F20:
        return SDLK_F20;
        break;
    case proj::KeyCode::F21:
        return SDLK_F21;
        break;
    case proj::KeyCode::F22:
        return SDLK_F22;
        break;
    case proj::KeyCode::F23:
        return SDLK_F23;
        break;
    case proj::KeyCode::F24:
        return SDLK_F24;
        break;
    case proj::KeyCode::Execute:
        return SDLK_EXECUTE;
        break;
    case proj::KeyCode::Help:
        return SDLK_HELP;
        break;
    case proj::KeyCode::Menu:
        return SDLK_MENU;
        break;
    case proj::KeyCode::Select:
        return SDLK_SELECT;
        break;
    case proj::KeyCode::Stop:
        return SDLK_STOP;
        break;
    case proj::KeyCode::Again:
        return SDLK_AGAIN;
        break;
    case proj::KeyCode::Undo:
        return SDLK_UNDO;
        break;
    case proj::KeyCode::Cut:
        return SDLK_CUT;
        break;
    case proj::KeyCode::Copy:
        return SDLK_COPY;
        break;
    case proj::KeyCode::Paste:
        return SDLK_PASTE;
        break;
    case proj::KeyCode::Find:
        return SDLK_FIND;
        break;
    case proj::KeyCode::Mute:
        return SDLK_MUTE;
        break;
    case proj::KeyCode::Volumeup:
        return SDLK_VOLUMEUP;
        break;
    case proj::KeyCode::Volumedown:
        return SDLK_VOLUMEDOWN;
        break;
    case proj::KeyCode::KP_Comma:
        return SDLK_KP_COMMA;
        break;
    case proj::KeyCode::KP_Equalsas400:
        return SDLK_KP_EQUALSAS400;
        break;

    case proj::KeyCode::Alterase:
        return SDLK_ALTERASE;
        break;
    case proj::KeyCode::Sysreq:
        return SDLK_SYSREQ;
        break;
    case proj::KeyCode::Cancel:
        return SDLK_CANCEL;
        break;
    case proj::KeyCode::Clear:
        return SDLK_CLEAR;
        break;
    case proj::KeyCode::Prior:
        return SDLK_PRIOR;
        break;
    case proj::KeyCode::Return2:
        return SDLK_RETURN2;
        break;
    case proj::KeyCode::Separator:
        return SDLK_SEPARATOR;
        break;
    case proj::KeyCode::Out:
        return SDLK_OUT;
        break;
    case proj::KeyCode::Oper:
        return SDLK_OPER;
        break;
    case proj::KeyCode::Clearagain:
        return SDLK_CLEARAGAIN;
        break;
    case proj::KeyCode::Crsel:
        return SDLK_CRSEL;
        break;
    case proj::KeyCode::Exsel:
        return SDLK_EXSEL;
        break;

    case proj::KeyCode::KP_00:
        return SDLK_KP_00;
        break;
    case proj::KeyCode::KP_000:
        return SDLK_KP_000;
        break;
    case proj::KeyCode::Thousandsseparator:
        return SDLK_THOUSANDSSEPARATOR;
        break;
    case proj::KeyCode::Decimalseparator:
        return SDLK_DECIMALSEPARATOR;
        break;
    case proj::KeyCode::Currencyunit:
        return SDLK_CURRENCYUNIT;
        break;
    case proj::KeyCode::Currencysubunit:
        return SDLK_CURRENCYSUBUNIT;
        break;
    case proj::KeyCode::KP_Leftparen:
        return SDLK_KP_LEFTPAREN;
        break;
    case proj::KeyCode::KP_Rightparen:
        return SDLK_KP_RIGHTPAREN;
        break;
    case proj::KeyCode::KP_Leftbrace:
        return SDLK_KP_LEFTBRACE;
        break;
    case proj::KeyCode::KP_Rightbrace:
        return SDLK_KP_RIGHTBRACE;
        break;
    case proj::KeyCode::KP_Tab:
        return SDLK_KP_TAB;
        break;
    case proj::KeyCode::KP_Backspace:
        return SDLK_KP_BACKSPACE;
        break;
    case proj::KeyCode::KP_A:
        return SDLK_KP_A;
        break;
    case proj::KeyCode::KP_B:
        return SDLK_KP_B;
        break;
    case proj::KeyCode::KP_C:
        return SDLK_KP_C;
        break;
    case proj::KeyCode::KP_D:
        return SDLK_KP_D;
        break;
    case proj::KeyCode::KP_E:
        return SDLK_KP_E;
        break;
    case proj::KeyCode::KP_F:
        return SDLK_KP_F;
        break;
    case proj::KeyCode::KP_Xor:
        return SDLK_KP_XOR;
        break;
    case proj::KeyCode::KP_Power:
        return SDLK_KP_POWER;
        break;
    case proj::KeyCode::KP_Percent:
        return SDLK_KP_PERCENT;
        break;
    case proj::KeyCode::KP_Less:
        return SDLK_KP_LESS;
        break;
    case proj::KeyCode::KP_Greater:
        return SDLK_KP_GREATER;
        break;
    case proj::KeyCode::KP_Ampersand:
        return SDLK_KP_AMPERSAND;
        break;
    case proj::KeyCode::KP_Dblampersand:
        return SDLK_KP_DBLAMPERSAND;
        break;
    case proj::KeyCode::KP_Verticalbar:
        return SDLK_KP_VERTICALBAR;
        break;
    case proj::KeyCode::KP_Dblverticalbar:
        return SDLK_KP_DBLVERTICALBAR;
        break;
    case proj::KeyCode::KP_Colon:
        return SDLK_KP_COLON;
        break;
    case proj::KeyCode::KP_Hash:
        return SDLK_KP_HASH;
        break;
    case proj::KeyCode::KP_Space:
        return SDLK_KP_SPACE;
        break;
    case proj::KeyCode::KP_At:
        return SDLK_KP_AT;
        break;
    case proj::KeyCode::KP_Exclam:
        return SDLK_KP_EXCLAM;
        break;
    case proj::KeyCode::KP_Memstore:
        return SDLK_KP_MEMSTORE;
        break;
    case proj::KeyCode::KP_Memrecall:
        return SDLK_KP_MEMRECALL;
        break;
    case proj::KeyCode::KP_Memclear:
        return SDLK_KP_MEMCLEAR;
        break;
    case proj::KeyCode::KP_Memadd:
        return SDLK_KP_MEMADD;
        break;
    case proj::KeyCode::KP_Memsubtract:
        return SDLK_KP_MEMSUBTRACT;
        break;
    case proj::KeyCode::KP_Memmultiply:
        return SDLK_KP_MEMMULTIPLY;
        break;
    case proj::KeyCode::KP_Memdivide:
        return SDLK_KP_MEMDIVIDE;
        break;
    case proj::KeyCode::KP_Plusminus:
        return SDLK_KP_PLUSMINUS;
        break;
    case proj::KeyCode::KP_Clear:
        return SDLK_KP_CLEAR;
        break;
    case proj::KeyCode::KP_Clearentry:
        return SDLK_KP_CLEARENTRY;
        break;
    case proj::KeyCode::KP_Binary:
        return SDLK_KP_BINARY;
        break;
    case proj::KeyCode::KP_Octal:
        return SDLK_KP_OCTAL;
        break;
    case proj::KeyCode::KP_Decimal:
        return SDLK_KP_DECIMAL;
        break;
    case proj::KeyCode::KP_Hexadecimal:
        return SDLK_KP_HEXADECIMAL;
        break;

    case proj::KeyCode::LCtrl:
        return SDLK_LCTRL;
        break;
    case proj::KeyCode::LShift:
        return SDLK_LSHIFT;
        break;
    case proj::KeyCode::LAlt:
        return SDLK_LALT;
        break;
    case proj::KeyCode::LGui:
        return SDLK_LGUI;
        break;
    case proj::KeyCode::RCtrl:
        return SDLK_RCTRL;
        break;
    case proj::KeyCode::RShift:
        return SDLK_RSHIFT;
        break;
    case proj::KeyCode::RAlt:
        return SDLK_RALT;
        break;
    case proj::KeyCode::RGUI:
        return SDLK_RGUI;
        break;

    case proj::KeyCode::Mode:
        return SDLK_MODE;
        break;

    case proj::KeyCode::Audionext:
        return SDLK_AUDIONEXT;
        break;
    case proj::KeyCode::Audioprev:
        return SDLK_AUDIOPREV;
        break;
    case proj::KeyCode::Audiostop:
        return SDLK_AUDIOSTOP;
        break;
    case proj::KeyCode::Audioplay:
        return SDLK_AUDIOPLAY;
        break;
    case proj::KeyCode::Audiomute:
        return SDLK_AUDIOMUTE;
        break;
    case proj::KeyCode::Mediaselect:
        return SDLK_MEDIASELECT;
        break;
    case proj::KeyCode::WWW:
        return SDLK_WWW;
        break;
    case proj::KeyCode::Mail:
        return SDLK_MAIL;
        break;
    case proj::KeyCode::Calculator:
        return SDLK_CALCULATOR;
        break;
    case proj::KeyCode::Computer:
        return SDLK_COMPUTER;
        break;
    case proj::KeyCode::AC_Search:
        return SDLK_AC_SEARCH;
        break;
    case proj::KeyCode::AC_Home:
        return SDLK_AC_HOME;
        break;
    case proj::KeyCode::AC_Back:
        return SDLK_AC_BACK;
        break;
    case proj::KeyCode::AC_Forward:
        return SDLK_AC_FORWARD;
        break;
    case proj::KeyCode::AC_Stop:
        return SDLK_AC_STOP;
        break;
    case proj::KeyCode::AC_Refresh:
        return SDLK_AC_REFRESH;
        break;
    case proj::KeyCode::AC_Bookmarks:
        return SDLK_AC_BOOKMARKS;
        break;

    case proj::KeyCode::Brightnessdown:
        return SDLK_BRIGHTNESSDOWN;
        break;
    case proj::KeyCode::Brightnessup:
        return SDLK_BRIGHTNESSUP;
        break;
    case proj::KeyCode::Displayswitch:
        return SDLK_DISPLAYSWITCH;
        break;
    case proj::KeyCode::Kbdillumtoggle:
        return SDLK_KBDILLUMTOGGLE;
        break;
    case proj::KeyCode::Kbdillumdown:
        return SDLK_KBDILLUMDOWN;
        break;
    case proj::KeyCode::Kbdillumup:
        return SDLK_KBDILLUMUP;
        break;
    case proj::KeyCode::Eject:
        return SDLK_EJECT;
        break;
    case proj::KeyCode::Sleep:
        return SDLK_SLEEP;
        break;
    case proj::KeyCode::App1:
        return SDLK_APP1;
        break;
    case proj::KeyCode::App2:
        return SDLK_APP2;
        break;

    case proj::KeyCode::Audiorewind:
        return SDLK_AUDIOREWIND;
        break;
    case proj::KeyCode::Audiofastforward:
        return SDLK_AUDIOFASTFORWARD;
        break;
    default:
        break;
    }

    return SDLK_UNKNOWN;
}

proj::KeyCode sdlKeyCodeToChar(SDL_Keycode kc)
{
    /* If kc is ASCII, there's no need to convert. */
    if(proj::isCharKey(static_cast<std::uint32_t>(kc)))
        return static_cast<proj::KeyCode>(kc);
    /* Huge lookup table for converting a  SDL_Keycode to an proj::KeyCode. */
    switch(kc)
    {
    case  SDLK_CAPSLOCK:
        return  proj::KeyCode::Capslock;
        break;

    case SDLK_F1:
        return  proj::KeyCode::F1;
        break;
    case SDLK_F2:
        return  proj::KeyCode::F2;
        break;
    case SDLK_F3:
        return  proj::KeyCode::F3;
        break;
    case SDLK_F4:
        return  proj::KeyCode::F4;
        break;
    case SDLK_F5:
        return proj::KeyCode::F5;
        break;
    case SDLK_F6:
        return proj::KeyCode::F6;
        break;
    case SDLK_F7:
        return proj::KeyCode::F7;
        break;
    case SDLK_F8:
        return proj::KeyCode::F8;
        break;
    case SDLK_F9:
        return proj::KeyCode::F9;
        break;
    case SDLK_F10:
        return proj::KeyCode::F10;
        break;
    case SDLK_F11:
        return proj::KeyCode::F11;
        break;
    case SDLK_F12:
        return proj::KeyCode::F12;
        break;

    case SDLK_PRINTSCREEN:
        return proj::KeyCode::Printscreen;
        break;
    case SDLK_SCROLLLOCK:
        return proj::KeyCode::Scrolllock;
        break;
    case SDLK_PAUSE:
        return proj::KeyCode::Pause;
        break;
    case SDLK_INSERT:
        return proj::KeyCode::Insert;
        break;
    case SDLK_HOME:
        return proj::KeyCode::Home;
        break;
    case SDLK_PAGEUP:
        return proj::KeyCode::Pageup;
        break;
    case SDLK_END:
        return proj::KeyCode::End;
        break;
    case SDLK_PAGEDOWN:
        return proj::KeyCode::Pagedown;
        break;
    case SDLK_RIGHT:
        return proj::KeyCode::Right;
        break;
    case SDLK_LEFT:
        return proj::KeyCode::Left;
        break;
    case SDLK_DOWN:
        return proj::KeyCode::Down;
        break;
    case SDLK_UP:
        return proj::KeyCode::Up;
        break;

    case SDLK_NUMLOCKCLEAR:
        return proj::KeyCode::Numlockclear;
        break;
    case SDLK_KP_DIVIDE:
        return proj::KeyCode::KP_Divide;
        break;
    case SDLK_KP_MULTIPLY:
        return proj::KeyCode::KP_Multiply;
        break;
    case SDLK_KP_MINUS:
        return proj::KeyCode::KP_Minus;
        break;
    case SDLK_KP_PLUS:
        return proj::KeyCode::KP_Plus;
        break;
    case SDLK_KP_ENTER:
        return proj::KeyCode::KP_Enter;
        break;
    case SDLK_KP_1:
        return proj::KeyCode::KP_1;
        break;
    case SDLK_KP_2:
        return proj::KeyCode::KP_2;
        break;
    case SDLK_KP_3:
        return proj::KeyCode::KP_3;
        break;
    case SDLK_KP_4:
        return proj::KeyCode::KP_4;
        break;
    case SDLK_KP_5:
        return proj::KeyCode::KP_5;
        break;
    case SDLK_KP_6:
        return proj::KeyCode::KP_6;
        break;
    case SDLK_KP_7:
        return proj::KeyCode::KP_7;
        break;
    case SDLK_KP_8:
        return proj::KeyCode::KP_8;
        break;
    case SDLK_KP_9:
        return proj::KeyCode::KP_9;
        break;
    case SDLK_KP_0:
        return proj::KeyCode::KP_0;
        break;
    case SDLK_KP_PERIOD:
        return proj::KeyCode::KP_Period;
        break;

    case SDLK_APPLICATION:
        return proj::KeyCode::Application;
        break;
    case SDLK_POWER:
        return proj::KeyCode::Power;
        break;
    case SDLK_KP_EQUALS:
        return proj::KeyCode::KP_Equals;
        break;
    case SDLK_F13:
        return proj::KeyCode::F13;
        break;
    case SDLK_F14:
        return proj::KeyCode::F14;
        break;
    case SDLK_F15:
        return proj::KeyCode::F15;
        break;
    case SDLK_F16:
        return proj::KeyCode::F16;
        break;
    case SDLK_F17:
        return proj::KeyCode::F17;
        break;
    case SDLK_F18:
        return proj::KeyCode::F18;
        break;
    case SDLK_F19:
        return proj::KeyCode::F19;
        break;
    case SDLK_F20:
        return proj::KeyCode::F20;
        break;
    case SDLK_F21:
        return proj::KeyCode::F21;
        break;
    case SDLK_F22:
        return proj::KeyCode::F22;
        break;
    case SDLK_F23:
        return proj::KeyCode::F23;
        break;
    case SDLK_F24:
        return proj::KeyCode::F24;
        break;
    case SDLK_EXECUTE:
        return proj::KeyCode::Execute;
        break;
    case SDLK_HELP:
        return proj::KeyCode::Help;
        break;
    case SDLK_MENU:
        return proj::KeyCode::Menu;
        break;
    case SDLK_SELECT:
        return proj::KeyCode::Select;
        break;
    case SDLK_STOP:
        return proj::KeyCode::Stop;
        break;
    case SDLK_AGAIN:
        return proj::KeyCode::Again;
        break;
    case SDLK_UNDO:
        return proj::KeyCode::Undo;
        break;
    case SDLK_CUT:
        return proj::KeyCode::Cut;
        break;
    case SDLK_COPY:
        return proj::KeyCode::Copy;
        break;
    case SDLK_PASTE:
        return proj::KeyCode::Paste;
        break;
    case SDLK_FIND:
        return proj::KeyCode::Find;
        break;
    case SDLK_MUTE:
        return proj::KeyCode::Mute;
        break;
    case SDLK_VOLUMEUP:
        return proj::KeyCode::Volumeup;
        break;
    case SDLK_VOLUMEDOWN:
        return proj::KeyCode::Volumedown;
        break;
    case SDLK_KP_COMMA:
        return proj::KeyCode::KP_Comma;
        break;
    case SDLK_KP_EQUALSAS400:
        return proj::KeyCode::KP_Equalsas400;
        break;

    case SDLK_ALTERASE:
        return proj::KeyCode::Alterase;
        break;
    case SDLK_SYSREQ:
        return proj::KeyCode::Sysreq;
        break;
    case SDLK_CANCEL:
        return proj::KeyCode::Cancel;
        break;
    case SDLK_CLEAR:
        return proj::KeyCode::Clear;
        break;
    case SDLK_PRIOR:
        return proj::KeyCode::Prior;
        break;
    case SDLK_RETURN2:
        return proj::KeyCode::Return2;
        break;
    case SDLK_SEPARATOR:
        return proj::KeyCode::Separator;
        break;
    case SDLK_OUT:
        return proj::KeyCode::Out;
        break;
    case SDLK_OPER:
        return proj::KeyCode::Oper;
        break;
    case SDLK_CLEARAGAIN:
        return proj::KeyCode::Clearagain;
        break;
    case SDLK_CRSEL:
        return proj::KeyCode::Crsel;
        break;
    case SDLK_EXSEL:
        return proj::KeyCode::Exsel;
        break;

    case SDLK_KP_00:
        return proj::KeyCode::KP_00;
        break;
    case SDLK_KP_000:
        return proj::KeyCode::KP_000;
        break;
    case SDLK_THOUSANDSSEPARATOR:
        return proj::KeyCode::Thousandsseparator;
        break;
    case SDLK_DECIMALSEPARATOR:
        return proj::KeyCode::Decimalseparator;
        break;
    case SDLK_CURRENCYUNIT:
        return proj::KeyCode::Currencyunit;
        break;
    case SDLK_CURRENCYSUBUNIT:
        return proj::KeyCode::Currencysubunit;
        break;
    case SDLK_KP_LEFTPAREN:
        return proj::KeyCode::KP_Leftparen;
        break;
    case SDLK_KP_RIGHTPAREN:
        return proj::KeyCode::KP_Rightparen;
        break;
    case SDLK_KP_LEFTBRACE:
        return proj::KeyCode::KP_Leftbrace;
        break;
    case SDLK_KP_RIGHTBRACE:
        return proj::KeyCode::KP_Rightbrace;
        break;
    case SDLK_KP_TAB:
        return proj::KeyCode::KP_Tab;
        break;
    case SDLK_KP_BACKSPACE:
        return proj::KeyCode::KP_Backspace;
        break;
    case SDLK_KP_A:
        return proj::KeyCode::KP_A;
        break;
    case SDLK_KP_B:
        return proj::KeyCode::KP_B;
        break;
    case SDLK_KP_C:
        return proj::KeyCode::KP_C;
        break;
    case SDLK_KP_D:
        return proj::KeyCode::KP_D;
        break;
    case SDLK_KP_E:
        return proj::KeyCode::KP_E;
        break;
    case SDLK_KP_F:
        return proj::KeyCode::KP_F;
        break;
    case SDLK_KP_XOR:
        return proj::KeyCode::KP_Xor;
        break;
    case SDLK_KP_POWER:
        return proj::KeyCode::KP_Power;
        break;
    case SDLK_KP_PERCENT:
        return proj::KeyCode::KP_Percent;
        break;
    case SDLK_KP_LESS:
        return proj::KeyCode::KP_Less;
        break;
    case SDLK_KP_GREATER:
        return proj::KeyCode::KP_Greater;
        break;
    case SDLK_KP_AMPERSAND:
        return proj::KeyCode::KP_Ampersand;
        break;
    case SDLK_KP_DBLAMPERSAND:
        return proj::KeyCode::KP_Dblampersand;
        break;
    case SDLK_KP_VERTICALBAR:
        return proj::KeyCode::KP_Verticalbar;
        break;
    case SDLK_KP_DBLVERTICALBAR:
        return proj::KeyCode::KP_Dblverticalbar;
        break;
    case SDLK_KP_COLON:
        return proj::KeyCode::KP_Colon;
        break;
    case SDLK_KP_HASH:
        return proj::KeyCode::KP_Hash;
        break;
    case SDLK_KP_SPACE:
        return proj::KeyCode::KP_Space;
        break;
    case SDLK_KP_AT:
        return proj::KeyCode::KP_At;
        break;
    case SDLK_KP_EXCLAM:
        return proj::KeyCode::KP_Exclam;
        break;
    case SDLK_KP_MEMSTORE:
        return proj::KeyCode::KP_Memstore;
        break;
    case SDLK_KP_MEMRECALL:
        return proj::KeyCode::KP_Memrecall;
        break;
    case SDLK_KP_MEMCLEAR:
        return proj::KeyCode::KP_Memclear;
        break;
    case SDLK_KP_MEMADD:
        return proj::KeyCode::KP_Memadd;
        break;
    case SDLK_KP_MEMSUBTRACT:
        return proj::KeyCode::KP_Memsubtract;
        break;
    case SDLK_KP_MEMMULTIPLY:
        return proj::KeyCode::KP_Memmultiply;
        break;
    case SDLK_KP_MEMDIVIDE:
        return proj::KeyCode::KP_Memdivide;
        break;
    case SDLK_KP_PLUSMINUS:
        return proj::KeyCode::KP_Plusminus;
        break;
    case SDLK_KP_CLEAR:
        return proj::KeyCode::KP_Clear;
        break;
    case SDLK_KP_CLEARENTRY:
        return proj::KeyCode::KP_Clearentry;
        break;
    case SDLK_KP_BINARY:
        return proj::KeyCode::KP_Binary;
        break;
    case SDLK_KP_OCTAL:
        return proj::KeyCode::KP_Octal;
        break;
    case SDLK_KP_DECIMAL:
        return proj::KeyCode::KP_Decimal;
        break;
    case SDLK_KP_HEXADECIMAL:
        return proj::KeyCode::KP_Hexadecimal;
        break;

    case SDLK_LCTRL:
        return proj::KeyCode::LCtrl;
        break;
    case SDLK_LSHIFT:
        return proj::KeyCode::LShift;
        break;
    case SDLK_LALT:
        return proj::KeyCode::LAlt;
        break;
    case SDLK_LGUI:
        return proj::KeyCode::LGui;
        break;
    case SDLK_RCTRL:
        return proj::KeyCode::RCtrl;
        break;
    case SDLK_RSHIFT:
        return proj::KeyCode::RShift;
        break;
    case SDLK_RALT:
        return proj::KeyCode::RAlt;
        break;
    case SDLK_RGUI:
        return proj::KeyCode::RGUI;
        break;

    case SDLK_MODE:
        return proj::KeyCode::Mode;
        break;

    case SDLK_AUDIONEXT:
        return proj::KeyCode::Audionext;
        break;
    case SDLK_AUDIOPREV:
        return proj::KeyCode::Audioprev;
        break;
    case SDLK_AUDIOSTOP:
        return proj::KeyCode::Audiostop;
        break;
    case SDLK_AUDIOPLAY:
        return proj::KeyCode::Audioplay;
        break;
    case SDLK_AUDIOMUTE:
        return proj::KeyCode::Audiomute;
        break;
    case SDLK_MEDIASELECT:
        return proj::KeyCode::Mediaselect;
        break;
    case SDLK_WWW:
        return proj::KeyCode::WWW;
        break;
    case SDLK_MAIL:
        return proj::KeyCode::Mail;
        break;
    case SDLK_CALCULATOR:
        return proj::KeyCode::Calculator;
        break;
    case SDLK_COMPUTER:
        return proj::KeyCode::Computer;
        break;
    case SDLK_AC_SEARCH:
        return proj::KeyCode::AC_Search;
        break;
    case SDLK_AC_HOME:
        return proj::KeyCode::AC_Home;
        break;
    case SDLK_AC_BACK:
        return proj::KeyCode::AC_Back;
        break;
    case SDLK_AC_FORWARD:
        return proj::KeyCode::AC_Forward;
    case SDLK_AC_STOP:
        return proj::KeyCode::AC_Stop;
        break;
    case SDLK_AC_REFRESH:
        return proj::KeyCode::AC_Refresh;
        break;
    case SDLK_AC_BOOKMARKS:
        return proj::KeyCode::AC_Bookmarks;
        break;

    case SDLK_BRIGHTNESSDOWN:
        return proj::KeyCode::Brightnessdown;
        break;
    case SDLK_BRIGHTNESSUP:
        return proj::KeyCode::Brightnessup;
        break;
    case SDLK_DISPLAYSWITCH:
        return proj::KeyCode::Displayswitch;
        break;
    case SDLK_KBDILLUMTOGGLE:
        return proj::KeyCode::Kbdillumtoggle;
        break;
    case SDLK_KBDILLUMDOWN:
        return proj::KeyCode::Kbdillumdown;
        break;
    case SDLK_KBDILLUMUP:
        return proj::KeyCode::Kbdillumup;
        break;
    case SDLK_EJECT:
        return proj::KeyCode::Eject;
        break;
    case SDLK_SLEEP:
        return proj::KeyCode::Sleep;
        break;
    case SDLK_APP1:
        return proj::KeyCode::App1;
        break;
    case SDLK_APP2:
        return proj::KeyCode::App2;
        break;

    case SDLK_AUDIOREWIND:
        return proj::KeyCode::Audiorewind;
        break;
    case SDLK_AUDIOFASTFORWARD:
        return proj::KeyCode::Audiofastforward;
        break;
    default:
        break;
    }

    return proj::KeyCode::Unknown;
}
