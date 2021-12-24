#include "gameLayer.hpp"

namespace chron = std::chrono;
using namespace std::chrono_literals;

/// The amount of time it takes for a single logical frame to pass.
/// 1/35 of a second (this is integral to the workings of doom).
static chron::nanoseconds logicalFrameTime = 28571ns;

doom::GameLayer::GameLayer()
    : frame::Layer(logicalFrameTime)
{
}

void doom::GameLayer::update()
{
}
