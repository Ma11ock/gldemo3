#include <iostream>
#include <cstdint>
#include <string>
#include <vector>

#include "renderer/renderer.hpp"
#include "frame.hpp"
#include "input.hpp"

using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;
static std::vector<std::string> args;

constexpr std::string_view NAME = "project";

int main(int argc, const char * const argv[])
{
    int result = EXIT_SUCCESS;
    try
    {
        args = std::vector<std::string>(argv + 1, argv + argc);
        rndr::init("project", 1200, 900);
        //
        frame::init();

        while(true)
        {
            input::pollInput();
            rndr::clearWindow();
            frame::start();

            // Draw

            rndr::present();
            if(input::tryQuit())
                break;

            frame::end();
        }
    }
    catch(const std::exception &e)
    {
        std::cerr << "Fatal exception: " << e.what() << '\n';
        result = EXIT_FAILURE;
    }

    rndr::quit();
    return result;
}

