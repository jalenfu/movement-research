#include "entity.hpp"
#include "library.hpp"
#include <iostream>

void Entity::update()
{ 
    posX += velX;

    if (posY > 400)
    {
        velX = 0;
    }

    if( ( posX < 0 ) || ( posX + width > SCREEN_WIDTH ) )
    {
        posX -= velX;
    }

    posY += velY;

    if (posY + height < SCREEN_HEIGHT)
    {
        velY += gravity;
    }

    if( ( posY < 0 ) || ( posY + height > SCREEN_HEIGHT ) )
    {
        posY -= velY;
        velY = 0;
    }

    if (posX != lastPosX || posY != lastPosY)
    {
        std::cout << "Position: " << posX << " " << posY << std::endl;
    }
    if (velX != lastVelX || velY != lastVelY)
    {
        std::cout << "Velocity: " << velX << " " << velY << std::endl;
    }

    lastPosX = posX;
    lastPosY = posY;
    lastVelX = velX;
    lastVelY = velY;
}

void Entity::render()
{
    texture.render(posX, posY);
}