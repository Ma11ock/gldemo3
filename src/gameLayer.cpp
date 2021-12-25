#include "gameLayer.hpp"

namespace chron = std::chrono;
using namespace std::chrono_literals;

#include "mouseEvent.hpp"
#include "graphics.hpp"
#include "keyboardEvent.hpp"

const chron::nanoseconds proj::GameLayer::LOGICAL_FRAME_TIME = 28570000ns;

proj::GameLayer::GameLayer()
    : frame::Layer(LOGICAL_FRAME_TIME),mRightButtonIsPressed(false),
      mMouseMoved(false),mLastMouseX(0.f),mLastMouseY(0.f),mXpos(0.f),mYpos(0.f)
{
}

void proj::GameLayer::update()
{
    if(mMouseMoved)
    {
        static bool firstMouse = true;
        if(firstMouse)
        {
            mLastMouseX = mXpos;
            mLastMouseY = mYpos;
            firstMouse = false;
        }

        float dx = mXpos - mLastMouseX;
        float dy = mLastMouseY - mYpos;

        mLastMouseX = mXpos;
        mLastMouseY = mYpos;
        if(mRightButtonIsPressed)
            graph::mouseMoved(dx, dy);
    }
}

void proj::GameLayer::draw(double alpha)
{
    
}


void proj::GameLayer::handleEvent(std::shared_ptr<proj::Event> event)
{
    switch(event->getEventType())
    {
    case proj::EventType::MouseMoved:
    {
        auto &mouseEvent = *dynamic_cast<proj::MouseMove*>(event.get());
        mXpos = mouseEvent.getX();
        mYpos = mouseEvent.getY();
        event->setHandled(true);
        mMouseMoved = true;
    }
        break;
    case proj::EventType::MousePressed:
    {
        auto &mouseEvent = *dynamic_cast<proj::MouseButton*>(event.get());
        switch(mouseEvent.getMouseCode())
        {
        case proj::MouseCode::Right:
            mRightButtonIsPressed = true;
            break;
        default:
            break;
        }
    }
        break;
    default:
        break;
    }
}

void proj::GameLayer::startFrame()
{
    mRightButtonIsPressed = false;
    mMouseMoved = false;
}
