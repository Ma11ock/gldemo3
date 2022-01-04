#include "InputMap.hpp"

proj::InputMap::InputMap(const std::vector<std::pair<proj::KeyCode,
                         std::function<bool()>>> &keybinds)
{
    std::sort(mKeybinds.begin(), mKeybinds.end(),
              [](std::pair<proj::KeyCode, std::function<bool()>> i,
                 std::pair<proj::KeyCode, std::function<bool()>> j)
              {
                  return static_cast<std::uint32_t>(i.first) < static_cast<std::uint32_t>(j.first);
              });
    for(unsigned int i = 0; i < mKeybinds.size(); i++)
    {
        if(mKeybinds[i] == mKeybinds[i + 1])
            mKeybinds.erase(mKeybinds.begin() + i);
    }
}

bool proj::InputMap::addKeybind(std::pair<proj::KeyCode,
                                std::function<bool()>> keybind)
{
    if(auto it = std::find(mKeybinds.begin(), mKeybinds.end(), keybind);
       it != mKeybinds.end())
    {
        *it = keybind;
        return true;
    }
    else
        mKeybinds.push_back(keybind);
    return false;
}
