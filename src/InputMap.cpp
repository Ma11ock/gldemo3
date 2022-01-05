#include "InputMap.hpp"

bool proj::InputMap::addKeybind(const proj::InputMap::mapType &keybind)
{
    bool isDup = getKeymapping(keybind.code) != mKeybinds.end();
    mKeybinds.push_back(keybind);
    return isDup;
}

bool proj::InputMap::doKeybind(proj::KeyCode keyPressed)
{
    if(auto it = getKeymapping(keyPressed); it != mKeybinds.end())
        return it->func();
    return false;
}

// TODO key events: when a key event exists right now, like temporary interact.
