#pragma once
#include <SDL.h>
#include "entity.hpp"
#include "input_handler.hpp"

class Player : virtual public Entity
{
public:
    Player();
    ~Player();

    void handleEvent( InputHandler& inputHandler );
    void update( InputHandler& inputHandler );
    
    // New methods for analog input support
    void setAnalogInput(double leftStickX, double leftStickY);
    void setAnalogSensitivity(double sensitivity) { analogSensitivity = sensitivity; }
    double getAnalogSensitivity() const { return analogSensitivity; }
    
    // Jump state management
    void resetJumpStates();

private:
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
};

/*
    handleEvent fucking sucks. Add methods to add/remove/set velocity. Adds flexibility for different forms of input.
*/