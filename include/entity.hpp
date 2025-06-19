#pragma once
#include "ltexture.hpp"
#include <SDL.h>
#include <vector>

enum EntityState
{
    waiting,
    walk,
    run,
    dash,
    jump1,
    fall
};

// Platform structure
struct Platform
{
    int x, y, width, height;
    Platform(int x, int y, int w, int h) : x(x), y(y), width(w), height(h) {}
};

class Entity
{
protected:
    double gravity, friction;
    int posX, posY;
    double velX, velY;
    double accelX, accelY;
    double maxVelX, maxVelY;
    int width, height;
    EntityState state;
    LTexture texture;
    
    // Platform support
    static std::vector<Platform> platforms;
    bool onPlatform;
    int currentPlatform;
    bool standingOnPlatform; // Track if currently standing on platform
    static bool passThroughPlatforms; // Flag to ignore platform collisions

public:
    void render();
    void handleEvent( SDL_Event& e );
    virtual void update();
    
    // Dash-related methods
    void setVelocityClamping(bool enabled) { velocityClampingEnabled = enabled; }
    bool isVelocityClampingEnabled() const { return velocityClampingEnabled; }
    
    // Platform methods
    static void addPlatform(int x, int y, int width, int height);
    static void clearPlatforms();
    static const std::vector<Platform>& getPlatforms() { return platforms; }
    static void setPassThroughPlatforms(bool passThrough) { passThroughPlatforms = passThrough; }
    bool checkPlatformCollision();
    bool isOnGroundOrPlatform() const;
    
private:
    int lastPosX, lastPosY, lastVelX, lastVelY;
    bool velocityClampingEnabled;
};