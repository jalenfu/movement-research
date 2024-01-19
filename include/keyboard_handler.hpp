#pragma once
#include <unordered_map>
#include <SDL.h>

class KeyboardHandler
{
private:
    std::unordered_map<SDL_KeyCode, bool> keyPressed;
public:
    void handleKeyboardEvent(SDL_Event& e);

    bool isPressed(SDL_KeyCode k);
};