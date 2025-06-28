#pragma once
#include <vector>
#include <SDL.h>

struct Platform
{
    int x, y, width, height;
    Platform(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}
};

class PlatformManager
{
private:
    static std::vector<Platform> platforms;

public:
    static void addPlatform(int x, int y, int width, int height);
    static void clearPlatforms();
    static const std::vector<Platform>& getPlatforms() { return platforms; }
    
    // Platform collision detection
    static bool checkPlatformCollision(int posX, int posY, int width, int height, double velY, bool passThrough);
    static bool checkPlatformCollisionWithPrevious(int posX, int posY, int prevPosX, int prevPosY, int width, int height, double velY, bool passThrough);
    static bool isOnPlatform(int posX, int posY, int width, int height, double velY, bool passThrough);
}; 