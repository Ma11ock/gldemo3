/**
 * @brief The game implementation.
 * @author Ryan Jeffrey <ryan@ryanmj.xyz>
 * @date 08/7/21
*/
#ifndef PROJ_GAME_LAYER_HPP
#define PROJ_GAME_LAYER_HPP

#include "frame.hpp"
#include "graphics.hpp"


#include <cstdint>
#include <chrono>

namespace proj
{
    class GameLayer : public frame::Layer
    {
    public:
        const static std::chrono::nanoseconds LOGICAL_FRAME_TIME;
        GameLayer();
        virtual ~GameLayer() = default;

        virtual void update();
        virtual void draw(double alpha);
        virtual void handleEvent(std::shared_ptr<proj::Event> event);
        virtual void startFrame();
    protected:
        bool mRightButtonIsPressed;
        bool mMouseMoved;
        float mLastMouseX;
        float mLastMouseY;
        float mXpos;
        float mYpos;
    };
}
#endif /* PROJ_GAME_LAYER_HPP */
