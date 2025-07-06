#pragma once
#include "ltexture.hpp"
#include <SDL.h>

enum EntityState
{
    waiting,
    walk,
    run,
    dash,
    jump1,
    fall
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

public:
    void render();
    void handleEvent( SDL_Event& e );
    virtual void update();

private:
    int lastPosX, lastPosY, lastVelX, lastVelY;
};