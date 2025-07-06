#pragma once
#include <SDL.h>
#include "ltexture.hpp"
#include "input_handler.hpp"
#include "input_types.hpp"
#include "platform.hpp"
#include "target.hpp"

class Player
{
public:
    Player();
    virtual ~Player();

    // Pure virtual functions that must be implemented by derived classes
    virtual void handleEvent(InputHandler& inputHandler) = 0;
    virtual void update(InputHandler& inputHandler) = 0;
    
    // Common functionality that can be overridden if needed
    virtual void render();
    
    // Platform and target collision - common implementation
    bool checkPlatformCollision();
    bool isOnGroundOrPlatform() const;
    bool checkTargetCollision();
    void handleTargetCollision();
    
    // Getters for common properties
    int getPosX() const { return posX; }
    int getPosY() const { return posY; }
    double getVelX() const { return velX; }
    double getVelY() const { return velY; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    bool isOnGround() const { return onGround; }
    bool isOnPlatform() const { return onPlatform; }
    bool isStandingOnPlatform() const { return standingOnPlatform; }

protected:
    // Position and movement - common to both players
    int posX, posY;
    double velX, velY;
    int lastPosX, lastPosY;  // For continuous collision detection
    int width, height;
    
    // Physics - common to both players
    double gravity;
    
    // Platform state - common to both players
    bool onGround;
    bool onPlatform;
    bool standingOnPlatform;
    
    // Texture - common to both players
    LTexture texture;
    
    // Common helper methods
    void handleHorizontalBoundaryCollisions();
    void handleVerticalBoundaryCollisions();
    void handlePlatformCollisions(InputHandler& inputHandler);
    void applyGravity();
    void updateLastPosition();
    
    // Common collision detection helpers
    bool checkIfOnGround() const;
    void placeOnPlatform();
}; 