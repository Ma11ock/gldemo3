#ifndef DOOM_MOUSE_EVENT_HPP
#define DOOM_MOUSE_EVENT_HPP

#include "event.hpp"

namespace proj
{
    enum class MouseCode
    {
        None = 0,
        Left,
        Middle,
        Right,
        X1,
        X2,
    };

    class MouseMove : public proj::Event
    {
    public:
        virtual ~MouseMove() = default;
        MouseMove(float x, float y)
            : Event(proj::EventCategory::MouseInput, proj::EventType::MouseMoved),
              _x(x),_y(y)
        {
        }

        float getX() const
        { return _x; }

        float getY() const
        { return _y; }

        std::tuple<float, float> getPosition() const
        {
            return std::make_tuple(_x, _y);
        }
    protected:
        float _x;
        float _y;
    };

    class MouseButton : public proj::Event
    {
    public:
        virtual ~MouseButton() = default;

        MouseCode getMouseCode() const
        { return _code; }

        MouseButton(proj::EventType type, MouseCode code)
            : Event(proj::EventCategory::MouseButton, type),
              _code(code)
        {
        }

    protected:
        MouseCode _code;
    };

    class MouseWheel : public proj::Event
    {
    public:
        virtual ~MouseWheel() = default;

        MouseWheel(std::int32_t x, std::int32_t y)
            : Event(proj::EventCategory::MouseInput,
                    proj::EventType::MouseScrolled),
              _x(x),_y(y)
        {
        }

        inline std::int32_t getScrollX() const
        { return _x; }

        inline std::int32_t getScrollY() const
        { return _y; }

        inline std::tuple<std::int32_t, std::int32_t> getScroll() const
        { return std::make_tuple(_x, _y); }

    protected:
        std::int32_t _x;
        std::int32_t _y;
    };
}

#endif /* DOOM_MOUSE_EVENT_HPP */
