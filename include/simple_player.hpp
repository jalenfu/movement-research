#pragma once
#include <SDL.h>
#include "player.hpp"
#include "input_handler.hpp"
#include "input_types.hpp"

class SimplePlayer : public Player
{
public:
    SimplePlayer();
    ~SimplePlayer();

    void handleEvent(InputHandler& inputHandler) override;
    void update(InputHandler& inputHandler) override;
    
    // Wall jump helper
    bool canWallJumpNow() const;

private:
    // Hollow Knight style movement
    double moveSpeed;        // Constant horizontal movement speed
    double jumpSpeed;        // Initial jump velocity
    double doubleJumpSpeed;  // Double jump velocity (shorter than regular jump)
    double dashSpeed;        // Dash velocity
    double dashDuration;     // How long dash lasts (in frames)
    double dashCooldown;     // Cooldown between dashes (in frames)
    
    // Wall jump
    double wallJumpSpeedX;   // Horizontal wall jump velocity
    double wallJumpSpeedY;   // Vertical wall jump velocity
    bool canWallJump;        // Whether wall jump is available
    bool touchingWall;       // Whether touching a wall
    bool facingRight;        // Direction player is facing
    bool touchingLeftWall;   // Whether touching the left wall
    bool touchingRightWall;  // Whether touching the right wall
    bool lastTouchingLeftWall;  // Which wall was last touched (for buffer)
    bool lastTouchingRightWall; // Which wall was last touched (for buffer)
    bool inWallJump;         // Whether currently in wall jump
    int wallJumpFrames;      // Frames since wall jump
    int wallJumpControlDelay; // Frames before regaining control after wall jump
    int wallJumpBufferFrames; // Frames after leaving wall where wall jump is still possible
    int wallJumpBufferTimer;  // Timer for wall jump buffer
    
    // State tracking
    bool inDash;
    bool canDoubleJump;      // Whether double jump is available
    int dashFrames;
    int dashCooldownFrames;
    bool wasJumpPressed;
    bool wasDashPressed;
}; 