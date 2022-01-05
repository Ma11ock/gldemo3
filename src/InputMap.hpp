#ifndef INPUT_MAP_HPP
#define INPUT_MAP_HPP

#include <vector>
#include <functional>
#include <utility>
#include "event.hpp"

namespace proj
{
    class InputMap
    {
    public:
        struct mapType
        {
            std::string name;
            proj::KeyCode code;
            std::function<bool()> func;
        };
        // Default constructor.
        InputMap() = default;
        // Create input map with another map.
        InputMap(const std::vector<mapType> &keybinds)
            : mKeybinds(keybinds)
        {
        }
        // Default destructor.
        virtual ~InputMap() = default;
        // Add keybind. If the key is already present in the map,
        // simply add the keybind anyway.
        bool addKeybind(const mapType &keyBind);
        // Remove key from the map. If the key was present in the map return
        // true.
        bool removeKeybind(proj::KeyCode code);
        // Do keybinds. Return true if every key in the list was found in the map,
        // and all were satisfied.
        bool doKeybind(const std::vector<proj::KeyCode> &keysPressed);
        // Do keybind. Return true if the key was found in the map and
        // that the key was satisfied.
        bool doKeybind(proj::KeyCode keyPressed);

    private:
        std::vector<mapType> mKeybinds;
        inline std::vector<mapType>::iterator getKeymapping(proj::KeyCode code)
        {
            return std::find_if(mKeybinds.begin(), mKeybinds.end(),
                                [=](mapType map)
                                {
                                    return code == map.code;
                                });
        }
    };
}

#endif /* INPUT_MAP_HPP */
