#include "entity.hpp"
#include "library.hpp"
#include <iostream>
// #include <stdlib.h>

void Entity::update()
{ 
    // Apply friction against velocity, not acceleration
    if (velX != 0)
    {
        // Apply friction in the opposite direction of velocity
        if (velX > 0)
        {
            velX -= friction;
            if (velX < 0) velX = 0; // Prevent overshooting zero
        }
        else if (velX < 0)
        {
            velX += friction;
            if (velX > 0) velX = 0; // Prevent overshooting zero
        }
    }

    // Apply acceleration to velocity
    velX += accelX;

    // Clamp velocity to maximum (only if clamping is enabled)
    if (velocityClampingEnabled)
    {
        if (velX > maxVelX)
        {
            velX = maxVelX;
        }
        if (velX < -maxVelX)
        {
            velX = -maxVelX;
        }
    }

    // Update position
    posX += velX;

    // Handle horizontal boundary collisions
    if (posX < 0)
    {
        posX = 0;
        velX = 0; // Stop horizontal movement
    }
    else if (posX + width > SCREEN_WIDTH)
    {
        posX = SCREEN_WIDTH - width;
        velX = 0; // Stop horizontal movement
    }

    // Update vertical position
    posY += velY;

    // Handle vertical boundary collisions
    if (posY < 0)
    {
        posY = 0;
        velY = 0; // Stop upward movement
    }
    else if (posY + height > SCREEN_HEIGHT)
    {
        posY = SCREEN_HEIGHT - height; // Place exactly on ground
        velY = 0; // Stop downward movement
        accelY = 0; // Reset acceleration when on ground
    }

    // Check if on ground (at bottom of screen) - AFTER position update
    bool onGround = (posY + height >= SCREEN_HEIGHT);

    // Only apply gravity if not on ground
    if (!onGround)
    {
        velY += gravity;
    }
    else
    {
        accelY = 0; // Reset vertical acceleration when on ground
    }

    velY += accelY;

    lastPosX = posX;
    lastPosY = posY;
    lastVelX = velX;
    lastVelY = velY;
}

void Entity::render()
{
    texture.render(posX, posY);
}