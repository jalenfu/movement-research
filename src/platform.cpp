#include "platform.hpp"
#include "library.hpp"
#include <cstdlib>

// Initialize static platform vector
std::vector<Platform> PlatformManager::platforms;

void PlatformManager::addPlatform(int x, int y, int width, int height)
{
    platforms.push_back(Platform(x, y, width, height));
}

void PlatformManager::clearPlatforms()
{
    platforms.clear();
}

bool PlatformManager::checkPlatformCollision(int posX, int posY, int width, int height, double velY, bool passThrough)
{
    if (passThrough) {
        return false;
    }
    
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
            // Only trigger landing if falling
            if (velY > 0) {
                return true;
            }
        }
    }
    
    return false;
}

bool PlatformManager::checkPlatformCollisionWithPrevious(int posX, int posY, int prevPosX, int prevPosY, int width, int height, double velY, bool passThrough)
{
    if (passThrough) {
        return false;
    }
    
    // Check if player is on any platform
    for (int i = 0; i < platforms.size(); i++)
    {
        const Platform& platform = platforms[i];
        
        // Check current position
        if (posY + height >= platform.y && 
            posY + height <= platform.y + 2 && // Small tolerance
            posX + width > platform.x && 
            posX < platform.x + platform.width)
        {
            // Only trigger landing if falling
            if (velY > 0) {
                return true;
            }
        }
        
        // Check if player tunneled through the platform
        // This happens when the previous position was above the platform and current position is below
        if (velY > 0 && // Only check when falling
            prevPosY + height < platform.y && // Was above platform
            posY + height > platform.y && // Is now below platform
            posX + width > platform.x && 
            posX < platform.x + platform.width)
        {
            return true;
        }
    }
    
    return false;
}

bool PlatformManager::isOnPlatform(int posX, int posY, int width, int height, double velY, bool passThrough)
{
    if (passThrough) {
        return false;
    }
    
    // Check if player is on any platform
    for (const Platform& platform : platforms)
    {
        // Check if player is above the platform and within its bounds
        if (posY + height >= platform.y && 
            posY + height <= platform.y + 2 && // Small tolerance
            posX + width > platform.x && 
            posX < platform.x + platform.width)
        {
            // Consider standing on platform if not rising through it
            if (velY >= 0) {
                return true;
            }
        }
    }
    
    return false;
} 