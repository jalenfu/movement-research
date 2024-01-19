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

    texture.loadFromFile( "src/dot.bmp" );
}

Player::~Player()
{
    texture.free();
}

void Player::handleEvent(SDL_Event& e)
{
	if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_a: 
                accelX = -0.6;
                facingRight = false;
                break;
            case SDLK_d: 
                accelX = 0.6;
                facingRight = true;
                break;
            case SDLK_SPACE: 
                velY -= 10; 
                break;
            case SDLK_LSHIFT:
                state = dash;
                if (facingRight)
                {
                    velX += 10;
                }
                else
                {
                    velX -= 10;
                }
                break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_a:
            case SDLK_d: accelX = 0; break;
        }
    }
}