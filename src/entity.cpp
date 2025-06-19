#include "entity.hpp"
#include "library.hpp"
#include <iostream>
// #include <stdlib.h>

// Initialize static platform vector
std::vector<Platform> Entity::platforms;

// Initialize pass-through flag
bool Entity::passThroughPlatforms = false;

void Entity::addPlatform(int x, int y, int width, int height)
{
    platforms.push_back(Platform(x, y, width, height));
}

void Entity::clearPlatforms()
{
    platforms.clear();
}

bool Entity::checkPlatformCollision()
{
    // If pass-through is enabled, don't check collisions
    if (passThroughPlatforms) {
        onPlatform = false;
        currentPlatform = -1;
        standingOnPlatform = false;
        return false;
    }
    
    onPlatform = false;
    currentPlatform = -1;
    standingOnPlatform = false; // Reset standing state each frame
    
    // Check if player is on any platform
    for (int i = 0; i < platforms.size(); i++)
    {
        const Platform& platform = platforms[i];
        
        // Check if player is above the platform and within its bounds
        // Increase tolerance based on velocity to prevent tunneling
        int tolerance = 2 + std::max(0, (int)velY); // Base tolerance + velocity-based tolerance
        if (posY + height >= platform.y && 
            posY + height <= platform.y + tolerance && // Dynamic tolerance
            posX + width > platform.x && 
            posX < platform.x + platform.width)
        {
            // Check for landing (falling onto platform)
            if (velY > 0) {
                onPlatform = true;
                standingOnPlatform = true;
                currentPlatform = i;
                // Immediately place player exactly on platform to prevent double-layer
                posY = platform.y - height;
                return true;
            }
            // Check for standing on platform (zero velocity)
            else if (velY == 0) {
                standingOnPlatform = true;
                currentPlatform = i;
            }
        }
    }
    
    return onPlatform;
}

bool Entity::isOnGroundOrPlatform() const
{
    return (posY + height >= SCREEN_HEIGHT) || onPlatform;
}

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

    // Check platform collisions (will be overridden for pass-through in Player class)
    checkPlatformCollision();

    // Check if on ground (at bottom of screen) - AFTER position update
    bool onGround = (posY + height >= SCREEN_HEIGHT);

    // Handle platform landing - place player exactly on platform
    if (onPlatform && velY > 0) // Landing on platform
    {
        velY = 0; // Stop downward movement
    }

    // Only apply gravity if not on ground or platform
    if (!onGround && !standingOnPlatform)
    {
        velY += gravity;
    }
    else
    {
        accelY = 0; // Reset vertical acceleration when on ground or platform
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