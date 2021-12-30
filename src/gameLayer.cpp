#include "gameLayer.hpp"

#include <memory>
namespace chron = std::chrono;
using namespace std::chrono_literals;

#include "mouseEvent.hpp"
#include "graphics.hpp"
#include "keyboardEvent.hpp"
#include "renderer/Shader.hpp"
#include "renderer/Camera.hpp"

namespace
{
    std::shared_ptr<Shader> shaderProgram;
    std::shared_ptr<graph::Thing> claire;
    std::shared_ptr<graph::Thing> tyrant;
    std::shared_ptr<graph::Thing> leon;
    std::shared_ptr<graph::Thing> teapot;
    Camera camera(glm::vec3(30.f, 30.f, 30.f));
}

const chron::nanoseconds proj::GameLayer::LOGICAL_FRAME_TIME = 28570000ns;

proj::GameLayer::GameLayer()
    : frame::Layer(LOGICAL_FRAME_TIME),mRightButtonIsPressed(false),
      mMouseMoved(false),mLastMouseX(0.f),mLastMouseY(0.f),mXpos(0.f),
      mYpos(0.f)
{
    shaderProgram = std::make_shared<Shader>("shader/main.vert", "shader/main.frag");
    claire = std::make_shared<graph::Thing>("res/claire.obj", "res/claire.bmp",
                                            shaderProgram);
    tyrant = std::make_shared<graph::Thing>("res/tyrant.obj", "res/tyrant.png",
                                            shaderProgram);
    leon = std::make_shared<graph::Thing>("res/leon.obj", "res/leon.png",
                                          shaderProgram);
    teapot = std::make_shared<graph::Thing>("res/teapot.obj", "res/earth.png",
                                            shaderProgram);

    teapot->translate(glm::vec3(20.f, 0.f, 10.f));

    claire->translate(glm::vec3(0.f, 1.f, 0.f));

    tyrant->translate(glm::vec3(15.f, 0.f, 45.f));

    leon->translate(glm::vec3(-10.f, 0.f, 20.f));
    leon->rotate(glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));
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
            camera.processMouseMovement(dx, dy);
    }
}

void proj::GameLayer::draw(double alpha)
{
    shaderProgram->bind();
    auto persp = camera.createPerspective(1.f);

    glm::vec3 lightColor(1.f, 1.f, 1.f);

    shaderProgram->set("uViewMatrix", camera.getViewMatrix());
    shaderProgram->set("uProjectionMatrix", persp);
    shaderProgram->set("uTextureMatrix", glm::mat4(1.f));
    shaderProgram->set("uColorMatrix", glm::mat4(1.f));
    shaderProgram->set("uViewPos", camera.getPosition());
    shaderProgram->set("uNumDirLights", 1u);
    shaderProgram->set("uNumPointLights", 0u);
    shaderProgram->set("uNumSpotLights", 0u);
    shaderProgram->set("uDirLights[0].specular", lightColor * glm::vec3(1.f));
    shaderProgram->set("uDirLights[0].diffuse", lightColor * glm::vec3(1.f));
    shaderProgram->set("uDirLights[0].ambient", lightColor * glm::vec3(0.2f));
    shaderProgram->set("uDirLights[0].direction", -0.2f, -1.f, -0.3f);
    shaderProgram->set("uMaterial.diffuse", 0);
    shaderProgram->set("uMaterial.specular", 0);
    shaderProgram->set("uMaterial.shininess", 64.f);

    claire->draw(camera.getViewMatrix(), persp);
    tyrant->draw(camera.getViewMatrix(), persp);
    leon->draw(camera.getViewMatrix(), persp);
    teapot->draw(camera.getViewMatrix(), persp);
}


void proj::GameLayer::handleEvent(std::shared_ptr<proj::Event> event)
{
    // Find out which event type the event is, downcast to that type,
    // and then process it.
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
            event->setHandled(true);
            break;
        default:
            break;
        }
    }
    break;
    case proj::EventType::MouseScrolled:
    {
        auto &mouseEvent = *dynamic_cast<proj::MouseWheel*>(event.get());
        camera.processMouseScroll(static_cast<float>
                                  (mouseEvent.getScrollY()) / 10.f);
    }
    break;
    case proj::EventType::KeyPressed:
    case proj::EventType::KeyHold:
    {
        auto &keyboardEvent = *dynamic_cast<proj::KeyPressedEvent*>(event.get());

        event->setHandled(true);
        switch(keyboardEvent.getKeyCode())
        {
        case proj::KeyCode::W:
            camera.processKeyboard(Camera::Movement::Forward, mDeltaTime);
            break;
        case proj::KeyCode::A:
            camera.processKeyboard(Camera::Movement::Left, mDeltaTime);
            break;
        case proj::KeyCode::S:
            camera.processKeyboard(Camera::Movement::Backward, mDeltaTime);
            break;
        case proj::KeyCode::D:
            camera.processKeyboard(Camera::Movement::Right, mDeltaTime);
            break;
        default:
            event->setHandled(false);
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
