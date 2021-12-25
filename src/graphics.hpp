#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include <string>

namespace graph
{
    void init(const std::string &windowTitle, int width, int height);
    void quit();

    bool createWindowMsg(const std::string &msg);
    bool createWindowWarning(const std::string &msg);
    bool createWindowError(const std::string &msg);
    void clearWindow();
    void present();

    void mouseMoved(float x, float y); 
}

#endif /* GRAPHICS_HPP */
