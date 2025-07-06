#include "player.hpp"
#include "library.hpp"
#include <cmath>

Player::Player()
{
    posX = 0;
    posY = 300;
    lastPosX = posX;
    lastPosY = posY;
    
    velX = 0;
    velY = 0;
    width = 20;
    height = 20;
    
    gravity = 0.6; // Default gravity, can be overridden by derived classes
    
    onGround = false;
    onPlatform = false;
    standingOnPlatform = false;
    
    texture.loadFromFile("src/dot.bmp");
}

Player::~Player()
{
    texture.free();
}

void Player::render()
{
    texture.render(posX, posY);
}

bool Player::checkPlatformCollision()
{
    return onPlatform;
}

bool Player::isOnGroundOrPlatform() const
{
    return onGround || onPlatform;
}

bool Player::checkTargetCollision()
{
    return TargetManager::checkTargetCollision(posX, posY, width, height);
}

void Player::handleTargetCollision()
{
    TargetManager::handleTargetCollision(posX, posY, width, height);
}

void Player::handleHorizontalBoundaryCollisions()
{
    if (posX < 0)
    {
        posX = 0;
        velX = 0;
    }
    else if (posX + width > SCREEN_WIDTH)
    {
        posX = SCREEN_WIDTH - width;
        velX = 0;
    }
}

void Player::handleVerticalBoundaryCollisions()
{
    if (posY < 0)
    {
        posY = 0;
        velY = 0;
    }
    else if (posY + height > SCREEN_HEIGHT)
    {
        posY = SCREEN_HEIGHT - height;
        velY = 0;
        onGround = true;
    }
}

void Player::handlePlatformCollisions(InputHandler& inputHandler)
{
    // Check if holding down for platform pass-through
    bool holdingDown = false;
    
    // Check digital inputs for down
    for (Input i : inputHandler.getInputs()) {
        if (i == down) {
            holdingDown = true;
            break;
        }
    }
    
    // Check analog input for down
    if (inputHandler.isControllerConnected() && inputHandler.getLeftStickY() > 0.5) {
        holdingDown = true;
    }
    
    // Use the platform collision system
    onPlatform = PlatformManager::checkPlatformCollisionWithPrevious(posX, posY, lastPosX, lastPosY, width, height, velY, holdingDown);
    standingOnPlatform = PlatformManager::isOnPlatform(posX, posY, width, height, velY, holdingDown);
    
    // Handle platform landing - place player exactly on platform
    if (onPlatform && velY > 0) // Landing on platform
    {
        placeOnPlatform();
        velY = 0; // Stop downward movement
    }
}

void Player::applyGravity()
{
    // Check if on ground (at bottom of screen) - AFTER position update
    bool onGround = checkIfOnGround();

    // Only apply gravity if not on ground or platform
    if (!onGround && !standingOnPlatform)
    {
        velY += gravity;
    }
}

void Player::updateLastPosition()
{
    lastPosX = posX;
    lastPosY = posY;
}

bool Player::checkIfOnGround() const
{
    return (posY + height >= SCREEN_HEIGHT);
}

void Player::placeOnPlatform()
{
    // Find the platform we're landing on and place player on it
    for (const Platform& platform : PlatformManager::getPlatforms())
    {
        if (posX + width > platform.x && 
            posX < platform.x + platform.width)
        {
            // Check if we're currently on the platform surface
            if (posY + height >= platform.y && 
                posY + height <= platform.y + 2)
            {
                posY = platform.y - height; // Place exactly on platform
                break;
            }
            // Check if we tunneled through the platform
            else if (lastPosY + height < platform.y && 
                     posY + height > platform.y)
            {
                posY = platform.y - height; // Place exactly on platform
                break;
            }
        }
    }
} 