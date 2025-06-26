#include "player.hpp"

Player::Player()
{
    posX = 0;
    posY = 300;

    velX = 0;
    velY = 0;

    accelX = 0;
    accelY = 0;

    maxVelX = 10;
    maxVelY = 10;
    
    gravity = 0.23;
    width = 20;
    height = 20;

    facingRight = true;
    wasKeyPressed = {{SDLK_a, false},
                     {SDLK_d, false},
                     {SDLK_SPACE, false},
                     {SDLK_LSHIFT, false}};
    texture.loadFromFile( "src/dot.bmp" );
}

Player::~Player()
{
    texture.free();
}

void Player::handleEvent(InputHandler& k)
{
	for (Input i : k.getInputs()) {
        switch (i)
        {
        case left:
            accelX = -0.6;
            facingRight = false;
            break;
        case right:
            accelX = 0.6;
            facingRight = true;
            break;
        case up:
            accelY = -10;
            break;
        case shift:
            velX + (facingRight ? 10 : -10);
            break;
        default:
            break;
        }
    }
}

