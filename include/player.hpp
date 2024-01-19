#pragma once
#include <SDL.h>
#include "entity.hpp"
#include "keyboard_handler.hpp"

class Player : virtual public Entity
{
public:
    Player();
    ~Player();

    void handleEvent( KeyboardHandler& k );
private:
    bool facingRight;
    std::unordered_map<SDL_KeyCode, bool> wasKeyPressed;
};