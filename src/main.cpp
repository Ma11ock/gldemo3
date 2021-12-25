#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "graphics.hpp"
#include "frame.hpp"
#include "input.hpp"
#include "gameLayer.hpp"

using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

namespace
{
    std::vector<std::string> args;
}

int main(int argc, const char * const argv[])
{
    frame::Layer::addLayer(std::make_shared<proj::GameLayer>());
    int result = EXIT_SUCCESS;
    try
    {
        args = std::vector<std::string>(argv + 1, argv + argc);
        graph::init("project", 1200, 900);
        frame::init();

        bool playing = true;
        while(playing)
        {
            input::pollInput();
            graph::clearWindow();
            frame::start();

            // Draw

            graph::present();
            playing = !input::tryQuit();

            frame::end();
        }
    }
    catch(const std::exception &e)
    {
        if(!graph::createWindowError(e.what()))
            std::cerr << "Fatal: could not graphically display error message\n";
        std::cerr << "Fatal exception: " << e.what() << '\n';
        result = EXIT_FAILURE;
    }

    graph::quit();
    return result;
}
