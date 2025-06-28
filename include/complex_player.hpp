#pragma once
#include <SDL.h>
#include <unordered_map>
#include "ltexture.hpp"
#include "input_handler.hpp"
#include "input_types.hpp"
#include "platform.hpp"
#include "target.hpp"

class ComplexPlayer
{
public:
    ComplexPlayer();
    ~ComplexPlayer();

    void handleEvent(InputHandler& inputHandler);
    void update(InputHandler& inputHandler);
    void render();
    
    // New methods for analog input support
    void setAnalogInput(double leftStickX, double leftStickY);
    void setAnalogSensitivity(double sensitivity) { analogSensitivity = sensitivity; }
    double getAnalogSensitivity() const { return analogSensitivity; }
    
    // Jump state management
    void resetJumpStates();
    
    // Platform and target collision
    bool checkPlatformCollision();
    bool isOnGroundOrPlatform() const;
    bool checkTargetCollision();
    void handleTargetCollision();

private:
    // Position and movement
    int posX, posY;
    double velX, velY;
    double accelX, accelY;
    double maxVelX, maxVelY;
    int width, height;
    
    // Physics
    double gravity, friction;
    bool velocityClampingEnabled;
    
    // Direction
    bool facingRight;
    std::unordered_map<SDL_KeyCode, bool> wasKeyPressed;
    
    // Jump state tracking
    bool canJump;
    bool canDoubleJump;
    bool wasJumpPressed;
    bool isJumpHeld;
    bool canFastFall;
    
    // Jumpsquat and jump timing
    bool inJumpsquat;
    int jumpsquatFrames;
    int jumpsquatDuration;
    int jumpHoldFrames;
    int shortHopThreshold;
    int fullHopThreshold;
    
    // Dash state
    bool inDash;
    int dashFrames;
    int dashDuration;
    double dashVelocityX;
    double dashVelocityY;
    bool canDash;
    bool hasAirDash;
    bool wasDashPressed;
    
    // Analog input support
    double analogSensitivity;
    double currentAnalogX;
    double currentAnalogY;
    
    // Platform state
    bool onPlatform;
    int currentPlatform;
    bool standingOnPlatform;
    
    // Texture
    LTexture texture;
    
    // Last position/velocity for tracking
    int lastPosX, lastPosY, lastVelX, lastVelY;

    bool wasOnGroundOrPlatform;
}; 