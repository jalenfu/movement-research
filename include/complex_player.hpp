#pragma once
#include <SDL.h>
#include <unordered_map>
#include "player.hpp"
#include "input_handler.hpp"
#include "input_types.hpp"

class ComplexPlayer : public Player
{
public:
    ComplexPlayer();
    ~ComplexPlayer();

    void handleEvent(InputHandler& inputHandler) override;
    void update(InputHandler& inputHandler) override;
    
    // New methods for analog input support
    void setAnalogInput(double leftStickX, double leftStickY);
    void setAnalogSensitivity(double sensitivity) { analogSensitivity = sensitivity; }
    double getAnalogSensitivity() const { return analogSensitivity; }
    
    // Jump state management
    void resetJumpStates();

private:
    // Acceleration-based movement
    double accelX, accelY;
    double maxVelX, maxVelY;
    
    // Physics
    double friction;
    bool velocityClampingEnabled;
    
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
    
    // Last position/velocity for tracking
    int lastVelX, lastVelY;

    bool wasOnGroundOrPlatform;
}; 