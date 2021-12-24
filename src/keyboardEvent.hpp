#ifndef DOOM_KEYBOARD_EVENT_HPP
#define DOOM_KEYBOARD_EVENT_HPP

#include "event.hpp"
#include <cstdint>

namespace proj
{
    class KeyboardEvent : public proj::Event
    {
    public:
        virtual ~KeyboardEvent() = default;

        proj::KeyCode getKeyCode() const
        { return _keyPressed; }

        virtual proj::EventType getKeyState() const = 0;

    protected:
        proj::KeyCode _keyPressed;
        KeyboardEvent(proj::EventType type, proj::KeyCode kc = proj::KeyCode::Unknown)
            : Event(proj::EventCategory::KeyInput, type),
              _keyPressed(kc)
        {
        }
    };

    class KeyPressedEvent : public proj::KeyboardEvent
    {
    public:
        KeyPressedEvent(proj::EventType type = proj::EventType::KeyPressed,
                        proj::KeyCode kc = proj::KeyCode::Unknown)
            : proj::KeyboardEvent(type, kc)
        {}
        virtual ~KeyPressedEvent() = default;

        virtual proj::EventType getKeyState() const
        { return proj::EventType::KeyPressed; }
    };

    class KeyHoldEvent : public KeyPressedEvent
    {
    public:
        KeyHoldEvent(proj::KeyCode kc = proj::KeyCode::Unknown)
            : proj::KeyPressedEvent(proj::EventType::KeyHold, kc)
        {}
        virtual ~KeyHoldEvent() = default;

        virtual proj::EventType getKeyState() const
        { return proj::EventType::KeyHold; }
    };

    class KeyReleaseEvent : public proj::KeyboardEvent
    {
    public:
        KeyReleaseEvent(proj::KeyCode kc = proj::KeyCode::Unknown)
            : proj::KeyboardEvent(proj::EventType::KeyReleased, kc)
        {}
        virtual ~KeyReleaseEvent() = default;

        virtual proj::EventType getKeyState() const
        { return proj::EventType::KeyReleased; }
    };

    class KeyTextInput : public proj::KeyboardEvent
    {
    public:
        KeyTextInput(std::string_view str = "")
            : proj::KeyboardEvent(proj::EventType::KeyTextInput),_str(str)
        { }

        std::string getString() const
        { return _str; }

        virtual proj::EventType getKeyState() const
        { return proj::EventType::None; }
    protected:
        std::string _str;
    };
}

#endif /* DOOM_KEYBOARD_EVENT_HPP */
