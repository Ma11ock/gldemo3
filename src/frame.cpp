/**
 * @brief Frame and tick manager.
 * @author Ryan Jeffrey <ryan@ryanmj.xyz>
 * @data 08/7/21
 *
 *  The framerate and tickrate manager code (spread out across this file) are
 *  from https://stackoverflow.com/questions/59441699/gaffer-on-games-timestep-stdchrono-implementation
 *  which itself is a C++ implementation of https://gafferongames.com/post/fix_your_timestep/
 */
#include <chrono>
#include <thread>
#include <cstdint>
#include <string>
#include <array>
#include <iostream>
#include <memory>
#include <vector>
#include <tuple>
#include "frame.hpp"
#include "event.hpp"
#include "input.hpp"

using namespace std::literals;
namespace chron = std::chrono;

namespace
{
    /// The global frame counter, how many times the layers are drawn.
    std::uint32_t frames = 0;
    /// The global frame rate per second.
    std::uint32_t fps = 0;
    /// The global tickrate per second, how many times all the layers are ticked.
    std::uint64_t tps = 0;
    /// The global tick counter.
    std::uint64_t ticks = 0;
    /// The global total tick counter.
    std::uint64_t totalTicks = 0;


    frame::timePoint totalTime{};

    frame::timePoint currentTime = frame::sysClock::now();

    /// Contains a reference to each layer and its alpha value for each frame.
    struct layerData
    {
        std::shared_ptr<frame::Layer> layer;
        double alpha;
        frame::duration accumulator;
    };
    std::vector<layerData> layers;

    std::vector<std::shared_ptr<proj::Event>> eventQ;
}

/* Init the framerate system. */
void frame::init()
{
    // TODO be able to select framerate.
    // auto fps = proj::getSetting<std::int64_t>("fps");

    // double framerate = 300.0;
    // if(!fps)
    //     logerror << "Could not determine FPS... using default (300fps).\n";
    // else
    //     framerate = static_cast<double>(*fps);

    // ::timeUpdate = dnanoseconds(chron::duration_cast<dnanoseconds>(ONE_SECOND)
    //                             .count() / 100.0);
}

/* This method of frame rate and tick rate control uses 100% of the CPU like
   the original doom. */
void frame::start()
{
    constexpr auto MAX_FRAME_TIME = 250ms;

    for(auto &[layer, _, __] : layers)
        layer->startFrame();

    input::pollInput();

    /* Handle events. */
    for(auto event : eventQ)
        for(auto &[layer, _, __] : layers)
            if(!event->isHandled())
                layer->handleEvent(event);
    eventQ.clear();

    // Delta time.
    frame::timePoint newTime = frame::sysClock::now();
    auto frameTime = newTime - currentTime;
    if(frameTime > MAX_FRAME_TIME)
        frameTime = MAX_FRAME_TIME;
    currentTime = newTime;

    /* Handle events and tick from top to bottom. */
    for(auto &[layer, alpha, accumulator] : layers)
    {
        accumulator += frameTime;
        auto deltaTime = layer->getDeltaTime();
        if(deltaTime > 0ms)
        {
            while(accumulator >= deltaTime)
            {
                layer->update();
                accumulator -= deltaTime;
            }

            alpha = chron::duration<double>{accumulator} / deltaTime;
        }
        else
            layer->update();

        ticks++;
    }

    /* Draw from bottom to top. */
    for(auto &[layer, alpha, _] : layers)
        layer->draw(alpha);
    frames++;
}

void frame::end()
{
    auto static timer = frame::sysClock::now();
    if(frame::sysClock::now() - timer >= 1s)
    {
        fps = frames;
        tps = ticks;
        totalTicks += ticks;
        frames = 0;
        ticks = 0;
        timer += 1s;
    }
}

/* Get current game tick. */
std::uint64_t frame::getTick()
{
    return totalTicks;
}

void frame::Layer::addLayer(std::shared_ptr<frame::Layer> layer)
{
    layers.push_back(layerData{std::move(layer), 0.0, frame::duration{}});
}

frame::Layer::Layer()
    : frame::Layer::Layer(20ms, {})
{
}

frame::Layer::Layer(frame::duration deltaTime, frame::duration totalTime)
    : mDeltaTime(deltaTime),mTotalTime(totalTime),
      mTimer(frame::sysClock::now().time_since_epoch()),mTicks(0),mTps(0),
      mTotalTicks(0),mActive(true)
{
}

std::uint32_t frame::getGlobalFPS()
{
    return fps;
}

std::uint32_t frame::getGlobalFrame()
{
    return frames;
}

std::uint64_t frame::getGlobalTPS()
{
    return tps;
}

std::uint64_t frame::getGlobalTick()
{
    return ticks;
}

std::uint64_t frame::getGlobalTickCount()
{
    return totalTicks;
}

void frame::addToEventQueue(std::shared_ptr<proj::Event> event)
{
    eventQ.push_back(event);
}
