#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>

#ifdef _WIN32
extern "C" {
#include <windows.h>
#include <shellapi.h>
}
#include <locale>
#include <codecvt>
#endif // _WIN32

#include "graphics.hpp"
#include "frame.hpp"
#include "input.hpp"
#include "gameLayer.hpp"
#include "util.hpp"

using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;
namespace fs = std::filesystem;

namespace
{
    std::vector<std::string> args;
}

#ifdef _WIN32
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev,
                     PSTR cmdline, int cmdshow)
#else
int main(int argc, const char * const argv[])
#endif // _WIN32
{
    int result = EXIT_SUCCESS;
    std::cout << "Current working directory is " << fs::current_path() << '\n';
    try
    {
#ifdef _WIN32
        int argc = 0;
        auto argv = CommandLineToArgvW(GetCommandLineW(), &argc);
        if(argv != nullptr && argc >= 0)
        {
            // Convert wide strings to regular strings.
            using convertType = std::codecvt_utf8<wchar_t>;
            std::wstring_convert<convertType, wchar_t> converter;
            args.resize(argc);
            for(int i = 0; i < argc; i++)
                args[i] = converter.to_bytes(argv[i]);
        }
#else 
        args = std::vector<std::string>(argv + 1, argv + argc);
#endif // _WIN32
        graph::init("project", 1200, 900);
        frame::init();
        frame::Layer::addLayer(std::make_shared<proj::GameLayer>());

        bool playing = true;
        while(playing)
        {
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

