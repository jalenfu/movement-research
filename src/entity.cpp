#include "entity.hpp"
#include "library.hpp"
#include <iostream>
// #include <stdlib.h>

void Entity::update()
{ 
    velX += accelX;
    if (velX > maxVelX)
    {
        velX = maxVelX;
    }
    if (velX * -1 > maxVelX)
    {
        velX = -1 * maxVelX;
    }
    posX += velX;

    if( ( posX < 0 ) || ( posX + width > SCREEN_WIDTH ) )
    {
        posX -= velX;
    }

    posY += velY;

    if (posY + height <= SCREEN_HEIGHT)
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