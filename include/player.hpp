#pragma once
#include <SDL.h>
#include "entity.hpp"

class Player : virtual public Entity
{
public:
    Player();
    ~Player();

    void handleEvent( SDL_Event& e );
private:
    bool facingRight;
};