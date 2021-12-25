#include "graphics.hpp"

extern "C" {
#include <SDL2/SDL.h>
}

#include <string>

#include "renderer/renderer.hpp"

void graph::init(const std::string &windowTitle, int width,
                 int height)
{
    rndr::init(windowTitle, width, height);
}

void graph::quit()
{
    rndr::quit();
}

bool graph::createWindowError(const std::string &msg)
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                                    msg.c_str(),
                                    nullptr) == 0;
}

bool graph::createWindowWarning(const std::string &msg)
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_WARNING, "Warning",
                                    msg.c_str(),
                                    nullptr) == 0;
}

bool graph::createWindowMsg(const std::string &msg)
{
    return SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Information",
                                    msg.c_str(),
                                    nullptr) == 0;
}

void graph::clearWindow()
{
    rndr::clearWindow();
}

void graph::present()
{
    rndr::present();
}

void graph::mouseMoved(float x, float y)
{
    rndr::mouseMoved(x, y);
}
