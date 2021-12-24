/**
 * @brief The game implementation.
 * @author Ryan Jeffrey <ryan@ryanmj.xyz>
 * @date 08/7/21
*/
#ifndef DOOM_GAME_LAYER_HPP
#define DOOM_GAME_LAYER_HPP

#include "frame.hpp"


#include <cstdint>
#include <chrono>

namespace doom
{
    class GameLayer : public frame::Layer
    {
    public:
        GameLayer();
        virtual ~GameLayer() = default;

        virtual void update();
    };
}
#endif /* DOOM_GAME_LAYER_HPP */
