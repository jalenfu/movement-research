#pragma once
#include <vector>
#include <SDL.h>

struct Target
{
    int x, y, width, height;
    bool active;
    Target(int x, int y, int w, int h) : x(x), y(y), width(w), height(h), active(true) {}
};

class TargetManager
{
private:
    static std::vector<Target> targets;

public:
    static void addTarget(int x, int y, int width, int height);
    static void clearTargets();
    static const std::vector<Target>& getTargets() { return targets; }
    static void spawnRandomTarget();
    static bool checkTargetCollision(int posX, int posY, int width, int height);
    static void handleTargetCollision(int posX, int posY, int width, int height);
}; 