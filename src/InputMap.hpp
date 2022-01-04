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
        // Default constructor.
        InputMap() = default;
        // Create input map with another map.
        InputMap(const std::vector<std::pair<proj::KeyCode,
                 std::function<bool()>>> &keybinds);
        // Default destructor.
        virtual ~InputMap() = default;
        // Add keybind. If the key is already present in the map,
        // replace the old function with the new one. If the key was
        // already present in the map return true.
        bool addKeybind(std::pair<proj::KeyCode, std::function<bool()>> keybind);
        // Remove key from the map. If the key was present in the map return
        // true.
        bool removeKeybind(proj::KeyCode code);
        // Do keybinds. Return true if every key in the list was found in the map.
        bool doKeybind(const std::vector<proj::KeyCode> &keysPressed);
        // Do keybind. Return true if the key is present in the map.
        bool doKeybind(proj::KeyCode keyPressed);
    private:
        std::vector<std::pair<proj::KeyCode, std::function<bool()>>> mKeybinds;
    };
}

#endif /* INPUT_MAP_HPP */
