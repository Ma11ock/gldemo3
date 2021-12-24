#ifndef DOOM_INPUT_HPP
#define DOOM_INPUT_HPP

#include <tuple>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include "event.hpp"
#include "keyboardEvent.hpp"
#include "util.hpp"

namespace input
{
    /* Poll the operating system for input and create keyboard events. */
    void pollInput();
    /* Return true if the user is trying to quit. */
    bool tryQuit();
    /* Get the coordinates of the mouse (x,y). */
    std::tuple<float, float> getMousePos();
    /* Get the key state of a key. */
    proj::EventType getKeyState(char code,
                                const std::vector<proj::KeyModifiers> &mods = {});
    /* Get the key state of a key. */
    proj::EventType getKeyState(proj::KeyCode kc,
                                const std::vector<proj::KeyModifiers> &mods = {});
    /* Get a string of the last alphanumeric keypresses.
       Returns an empty string if no keypresses occurred. */
    std::string getAlphaNumericKeyPresses();
}

#endif /* DOOM_INPUT_HPP */
