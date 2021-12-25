#ifndef SCHOOLDOOM_FRAME_HPP
#define SCHOOLDOOM_FRAME_HPP

#include <chrono>
#include <functional>
#include <memory>

#include "event.hpp"

namespace frame
{
    using sysClock = std::chrono::system_clock;
    using duration = decltype(sysClock::duration{} + std::chrono::nanoseconds(1));
    using timePoint = std::chrono::time_point<sysClock, duration>;

    class Layer
    {
    public:
        // Add a layer to the queue.
        static void addLayer(std::shared_ptr<Layer> layer);

        Layer();
        Layer(frame::duration deltaTime, frame::duration totalTime = {});
        virtual ~Layer() = default;

        virtual void attach() { }
        virtual void update() { mTotalTime += mDeltaTime; }
        virtual void detach() { }
        virtual void draw(double alpha) { }
        virtual void startFrame() { }
        virtual void handleEvent(std::shared_ptr<proj::Event> event) { }

        inline frame::duration getDeltaTime() const
        { return mDeltaTime; }

        inline frame::duration getTotalTime() const
        { return mTotalTime; }

        inline bool isActive() const
        { return mActive; }

        inline void setActive(bool active)
        { mActive  = active; }

    protected:
        /// The time difference between each time update() is run.
        frame::duration mDeltaTime;
        /// The sum of every deltaTime when update() is run.
        frame::duration mTotalTime;
        /// Timer for keeping track of the tickrate.
        frame::duration mTimer;

        /// Number of ticks so far this second.
        std::uint64_t mTicks;
        /// The tickrate (per second).
        std::uint64_t mTps;
        /// The total amount of ticks.
        std::uint64_t mTotalTicks;

        /// The frame counter.
        std::uint32_t mFrames;
        /// The frame rate per second.
        std::uint32_t mFps;
        /// Whether or not the layer is active.
        bool mActive;
    };

    /* Init the framerate system. */
    void init();
    /* Start the frame. */
    void start();
    /* End the frame. */
    void end();
    /* Get current game tick. */
    std::uint64_t getTick();

    std::uint32_t getGlobalFPS();

    std::uint32_t getGlobalFrame();

    std::uint64_t getGlobalTPS();

    std::uint64_t getGlobalTick();

    std::uint64_t getGlobalTickCount();

    void addToEventQueue(std::shared_ptr<proj::Event> event);
}

#endif /* SCHOOLDOOM_FRAME_HPP */
