#include "target.hpp"
#include "platform.hpp"
#include "library.hpp"
#include <cstdlib>

// Initialize static target vector
std::vector<Target> TargetManager::targets;

void TargetManager::addTarget(int x, int y, int width, int height)
{
    targets.push_back(Target(x, y, width, height));
}

void TargetManager::clearTargets()
{
    targets.clear();
}

void TargetManager::spawnRandomTarget()
{
    // Clear existing targets
    targets.clear();
    
    // Generate random position (avoiding platforms)
    int targetX, targetY;
    bool validPosition = false;
    
    while (!validPosition) {
        targetX = rand() % (SCREEN_WIDTH - 30); // 30 is target width
        targetY = rand() % (SCREEN_HEIGHT - 100) + 50; // Avoid top and bottom edges
        
        // Check if position overlaps with platforms
        validPosition = true;
        for (const Platform& platform : PlatformManager::getPlatforms()) {
            if (targetX < platform.x + platform.width &&
                targetX + 30 > platform.x &&
                targetY < platform.y + platform.height &&
                targetY + 30 > platform.y) {
                validPosition = false;
                break;
            }
        }
    }
    
    // Add the target
    addTarget(targetX, targetY, 30, 30);
}

bool TargetManager::checkTargetCollision(int posX, int posY, int width, int height)
{
    for (auto& target : targets) {
        if (target.active &&
            posX < target.x + target.width &&
            posX + width > target.x &&
            posY < target.y + target.height &&
            posY + height > target.y) {
            // Player hit the target
            target.active = false;
            return true;
        }
    }
    return false;
}

void TargetManager::handleTargetCollision(int posX, int posY, int width, int height)
{
    if (checkTargetCollision(posX, posY, width, height)) {
        // Target was hit, spawn a new one
        spawnRandomTarget();
    }
} 