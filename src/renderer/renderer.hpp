#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>

namespace rndr
{
    void init(const std::string &title, int width, int height);
    void quit();
    void present();
    void clearWindow();
    void mouseMoved(float x, float y);
}

#endif /* RENDERER_HPP */
