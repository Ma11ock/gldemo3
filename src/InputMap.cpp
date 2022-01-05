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

bool proj::InputMap::doKeybind(const std::vector<proj::KeyCode> &keysPressed)
{
    bool result = true;
    for(auto code : keysPressed)
        result = doKeybind(code) && result;
    return result;
}

bool proj::InputMap::removeKeybind(proj::KeyCode code)
{
    if(auto it = getKeymapping(code);
       it != mKeybinds.end())
    {
        mKeybinds.erase(it);
        return true;
    }
    return false;
}

// TODO key events: when a key event exists right now, like temporary interact.
