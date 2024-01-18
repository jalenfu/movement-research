#pragma once
#include <SDL.h>

class Player
{
public:
    static const int PLAYER_WIDTH = 20;
    static const int PLAYER_HEIGHT = 20;

    Player();

    void handleEvent( SDL_Event& e);
    void move();
    void jump();
    void dash();
    void render();

private:
    int mPosX, mPosY;
    int mVelX, mVelY;
}