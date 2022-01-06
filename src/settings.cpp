#include "settings.hpp"

#include <unordered_map>

namespace
{
    using vecs = std::vector<std::string>;

    struct setting
    {
        std::variant<std::string, vecs, std::int64_t, double, bool> data;
        std::string descr;
    };

    std::unordered_map<std::string, setting> settings = 
    {
        { "screenMode", { vecs{"Fullscreen", "Windowed", "Fullscreen Windowed" },
            "Screen Mode"}},
        { "resolution", { vecs{"1200x900", "1920x1080" }, "Resultion"}},
        { "serverPort", { 27901, "Port number to connect to the server"}}
    };
}

void proj::init(const std::vector<std::string> &args)
{
    // TODO
}
