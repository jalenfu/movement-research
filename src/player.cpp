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

void Player::handleEvent(KeyboardHandler& k)
{
	if (!k.isPressed(SDLK_a) || !k.isPressed(SDLK_d))
    {
        accelX = 0;
    }
    if (k.isPressed(SDLK_a) && k.isPressed(SDLK_d))
    {
        accelX = 0;
    }
    else if (k.isPressed(SDLK_a))
    {
        accelX = -0.6;
        facingRight = false;
    }
    else if (k.isPressed(SDLK_d))
    {
        accelX = 0.6;
        facingRight = true;
    }
    if (!wasKeyPressed[SDLK_SPACE] && k.isPressed(SDLK_SPACE))
    {
        velY -= 10;
        wasKeyPressed[SDLK_SPACE] = true;
    }
    else if (!k.isPressed(SDLK_SPACE))
    {
        wasKeyPressed[SDLK_SPACE] = false;
    }
    if (!wasKeyPressed[SDLK_LSHIFT] && k.isPressed(SDLK_LSHIFT))
    {
        if (facingRight)
        {
            velX += 10;
        }
        else
        {
            velX -= 10;
        }
        wasKeyPressed[SDLK_LSHIFT] = true;
    }
    else if (!k.isPressed(SDLK_LSHIFT))
    {
        wasKeyPressed[SDLK_LSHIFT] = false;
    }
}