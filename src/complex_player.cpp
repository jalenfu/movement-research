#include "complex_player.hpp"
#include <cmath>
#include "library.hpp"

ComplexPlayer::ComplexPlayer() : Player()
{
    // Override gravity for ComplexPlayer
    gravity = 0.4;
    
    friction = 0.57;

    accelX = 0;
    accelY = 0;

    maxVelX = 10;
    maxVelY = 10;
    
    // Initialize jump states
    canJump = true;
    canDoubleJump = false;
    wasJumpPressed = false;
    isJumpHeld = false;
    canFastFall = false;
    
    // Initialize jumpsquat and jump timing
    inJumpsquat = false;
    jumpsquatFrames = 0;
    jumpsquatDuration = 3;  // 3 frames of jumpsquat
    jumpHoldFrames = 0;
    shortHopThreshold = 2;  // 2 frames or less for short hop
    fullHopThreshold = 3;   // 3 frames for full hop (all jumpsquat frames)
    
    // Initialize dash state
    inDash = false;
    dashFrames = 0;
    dashDuration = 8;  // 8 frames of dash (about 133ms at 60fps)
    dashVelocityX = 0.0;
    dashVelocityY = 0.0;
    canDash = true;
    hasAirDash = true;
    wasDashPressed = false;
    
    // Initialize analog input support
    analogSensitivity = 2.0;
    currentAnalogX = 0.0;
    currentAnalogY = 0.0;
    
    // Initialize velocity clamping (enabled by default)
    velocityClampingEnabled = true;
    
    // Initialize last position/velocity
    lastVelX = velX;
    lastVelY = velY;

    wasOnGroundOrPlatform = false;
}

ComplexPlayer::~ComplexPlayer()
{
    // Base class destructor handles texture cleanup
}

void ComplexPlayer::handleEvent(InputHandler& inputHandler)
{
    // Reset horizontal acceleration at the start of each frame
    accelX = 0;
    
    // Track input directions for dashing
    bool inputUp = false;
    bool inputDown = false;
    bool inputLeft = false;
    bool inputRight = false;
    isJumpHeld = false;
    
    // Handle digital inputs (keyboard + controller buttons)
	for (Input i : inputHandler.getInputs()) {
        switch (i)
        {
        case left:
            accelX = -2;
            inputLeft = true;
            break;
        case right:
            accelX = 2;
            inputRight = true;
            break;
        case up:
            isJumpHeld = true;
            // Handle jumping with double jump logic
            if (!wasJumpPressed) // Only trigger on initial press, not hold
            {
                if (canJump) // First jump (ground jump)
                {
                    // Start jumpsquat
                    inJumpsquat = true;
                    jumpsquatFrames = 0;
                    jumpHoldFrames = 0;
                    canJump = false;
                    canDoubleJump = true;
                    canFastFall = false;
                    printf("Started jumpsquat - canJump: %d, canDoubleJump: %d\n", canJump, canDoubleJump);
                }
                else if (canDoubleJump) // Second jump (air jump)
                {
                    // Air jumps don't have jumpsquat, immediate execution
            velY = -10;
            accelY = 0;
                    canDoubleJump = false;
                    canFastFall = false;
                    printf("Air jump executed: AIR JUMP (strength: -10.0)\n");
                }
            }
            wasJumpPressed = true;
            inputUp = true;
            break;
        case down:
            // Fast fall - increase downward velocity or cancel upward momentum
            if (canFastFall) {
                if (velY > 0)  // Already falling
                {
                    velY = std::min(velY + 5.0, 15.0);  // Increase fall speed, cap at 15
                }
                else if (velY < 0)  // Rising - cancel upward momentum
                {
                    velY = 5.0;  // Start falling immediately
                }
            }
            inputDown = true;
            break;
        case shift:
            // 8-directional dash based on input direction - Celeste style
            if (!wasDashPressed && canDash) // Only dash on initial press, not hold
            {
                bool onGround = (posY + height >= SCREEN_HEIGHT);
                
                if (onGround || hasAirDash) // Can dash on ground or if air dash available
                {
                    if (inputUp && inputLeft) {
                        // Up-left dash
                        dashVelocityX = -14.0;  // Stronger horizontal
                        dashVelocityY = -8.0;   // Weaker vertical
                    } else if (inputUp && inputRight) {
                        // Up-right dash
                        dashVelocityX = 14.0;   // Stronger horizontal
                        dashVelocityY = -8.0;   // Weaker vertical
                    } else if (inputDown && inputLeft) {
                        // Down-left dash
                        dashVelocityX = -14.0;  // Stronger horizontal
                        dashVelocityY = 8.0;    // Weaker vertical
                    } else if (inputDown && inputRight) {
                        // Down-right dash
                        dashVelocityX = 14.0;   // Stronger horizontal
                        dashVelocityY = 8.0;    // Weaker vertical
                    } else if (inputUp) {
                        // Up dash
                        dashVelocityX = 0.0;
                        dashVelocityY = -8.0;   // Weaker vertical
                    } else if (inputDown) {
                        // Down dash
                        dashVelocityX = 0.0;
                        dashVelocityY = 8.0;    // Weaker vertical
                    } else if (inputLeft) {
                        // Left dash
                        dashVelocityX = -14.0;  // Stronger horizontal
                        dashVelocityY = 0.0;
                    } else if (inputRight) {
                        // Right dash
                        dashVelocityX = 14.0;   // Stronger horizontal
                        dashVelocityY = 0.0;
                    } else {
                        // Default dash in current movement direction
                        dashVelocityX = (velX >= 0 ? 14.0 : -14.0);  // Stronger horizontal
                        dashVelocityY = 0.0;
                    }
                    
                    // Start dash state
                    inDash = true;
                    dashFrames = 0;
                    
                    // Disable velocity clamping during dash
                    velocityClampingEnabled = false;
                    
                    // Use air dash if in air
                    if (!onGround) {
                        hasAirDash = false;
                        printf("Air dash used - velocity: (%.1f, %.1f)\n", dashVelocityX, dashVelocityY);
                    } else {
                        printf("Ground dash - velocity: (%.1f, %.1f)\n", dashVelocityX, dashVelocityY);
                    }
                }
            }
            wasDashPressed = true;
            break;
        default:
            break;
        }
    }
    
    // Reset jump press state if jump button is not pressed
    if (!inputUp) {
        wasJumpPressed = false;
    }
    
    // Reset dash press state if dash button is not pressed
    if (!inputHandler.getInputs().count(shift)) {
        wasDashPressed = false;
    }
    
    // Handle analog input for more precise control
    if (inputHandler.isControllerConnected())
    {
        double leftStickX = inputHandler.getLeftStickX();
        double leftStickY = inputHandler.getLeftStickY();
        
        setAnalogInput(leftStickX, leftStickY);
        
        // Apply analog movement if no digital input is overriding it
        if (accelX == 0 && std::abs(leftStickX) > 0.1)
        {
            accelX = leftStickX * analogSensitivity;
        }
        
        // Apply analog jump if no digital input is overriding it
        if (leftStickY < -0.95 && canJump && !inJumpsquat && (posY + height >= SCREEN_HEIGHT))
        {
            // Start jumpsquat for analog jump
            inJumpsquat = true;
            jumpsquatFrames = 0;
            jumpHoldFrames = 0;
            canJump = false;
            canDoubleJump = true;
        }
        if (leftStickY < -0.95) {
            isJumpHeld = true;
        }
        
        // Apply analog fast fall if no digital input is overriding it
        if (leftStickY > 0.5 && canFastFall)  // Push down on stick
        {
            if (velY > 0)  // Already falling
            {
                velY = std::min(velY + 3.0, 15.0);  // Increase fall speed, cap at 15
            }
            else if (velY < 0)  // Rising - cancel upward momentum
            {
                velY = 3.0;  // Start falling immediately
            }
        }
    }

    // At the end of handleEvent, after all input and movement logic:
    static bool wasRising = false;
    if (velY < 0) {
        wasRising = true;
    } else if (velY > 0 && wasRising) {
        canFastFall = true;
        wasRising = false;
    }
}

void ComplexPlayer::setAnalogInput(double leftStickX, double leftStickY)
{
    currentAnalogX = leftStickX;
    currentAnalogY = leftStickY;
}

void ComplexPlayer::update(InputHandler& inputHandler)
{
    // Apply friction against velocity, not acceleration
    if (velX != 0)
    {
        // Apply friction in the opposite direction of velocity
        if (velX > 0)
        {
            velX -= friction;
            if (velX < 0) velX = 0; // Prevent overshooting zero
        }
        else if (velX < 0)
        {
            velX += friction;
            if (velX > 0) velX = 0; // Prevent overshooting zero
        }
    }

    // Apply acceleration to velocity
    velX += accelX;

    // Clamp velocity to maximum (only if clamping is enabled)
    if (velocityClampingEnabled)
    {
        if (velX > maxVelX)
        {
            velX = maxVelX;
        }
        if (velX < -maxVelX)
        {
            velX = -maxVelX;
        }
    }

    // Update position
    posX += velX;
    posY += velY;

    // Use base class collision handling
    handleHorizontalBoundaryCollisions();
    handleVerticalBoundaryCollisions();
    handlePlatformCollisions(inputHandler);
    
    // Apply gravity using base class method
    applyGravity();
    
    // Apply vertical acceleration
    velY += accelY;

    // Check target collisions and handle respawning
    handleTargetCollision();
    
    // Reset jump states after position update
    resetJumpStates();
    
    // Update last position/velocity
    updateLastPosition();
    lastVelX = velX;
    lastVelY = velY;

    // Update ground/platform state for next frame
    wasOnGroundOrPlatform = (posY + height >= SCREEN_HEIGHT) || standingOnPlatform;
}

void ComplexPlayer::resetJumpStates()
{
    // Check if on ground or platform (use existing onPlatform state from update)
    bool onGround = (posY + height >= SCREEN_HEIGHT);
    
    // Only reset jump states when transitioning from air to ground/platform
    if ((onGround || standingOnPlatform) && !wasOnGroundOrPlatform) // Just landed
    {
        canJump = true;
        canDoubleJump = false;
        // Reset jumpsquat state when on ground
        inJumpsquat = false;
        jumpsquatFrames = 0;
        jumpHoldFrames = 0;
        canFastFall = false;
        // Reset dash state when on ground
        inDash = false;
        dashFrames = 0;
        canDash = true;  // Can dash again on ground
        hasAirDash = true;  // Restore air dash
        // Ensure velocity clamping is enabled when not dashing
        velocityClampingEnabled = true;
        printf("Just landed - reset jump states - canJump: %d, canDoubleJump: %d\n", canJump, canDoubleJump);
    }
    
    // Handle jumpsquat state
    if (inJumpsquat)
    {
        jumpsquatFrames++;
        
        // Track jump hold duration if button is still pressed
        if (isJumpHeld)
        {
            jumpHoldFrames++;
        }
        
        printf("Jumpsquat frame: %d/%d, hold frames: %d\n", jumpsquatFrames, jumpsquatDuration, jumpHoldFrames);
        
        // Execute jump after jumpsquat duration
        if (jumpsquatFrames >= jumpsquatDuration)
        {
            // Determine jump strength based on how long button was held
            double jumpStrength;
            const char* jumpType;
            if (jumpHoldFrames <= shortHopThreshold)
            {
                jumpStrength = -7.0;  // Short hop
                jumpType = "SHORT HOP";
            }
            else if (jumpHoldFrames <= fullHopThreshold)
            {
                jumpStrength = -10.0;  // Full hop
                jumpType = "FULL HOP";
            }
            else
            {
                jumpStrength = -12.0;  // Maximum hop
                jumpType = "MAX HOP";
            }
            
            velY = jumpStrength;
            accelY = 0;
            
            // Exit jumpsquat state
            inJumpsquat = false;
            jumpsquatFrames = 0;
            jumpHoldFrames = 0;
            printf("Jump executed: %s (strength: %.1f)\n", jumpType, jumpStrength);
        }
    }
    
    // Handle dash state
    if (inDash)
    {
        dashFrames++;
        
        // Set velocity during dash
        velX = dashVelocityX;
        velY = dashVelocityY;
        
        printf("Dash frame: %d/%d, velocity: (%.1f, %.1f)\n", dashFrames, dashDuration, velX, velY);
        
        // End dash after duration
        if (dashFrames >= dashDuration)
        {
            // Keep the final velocity (Celeste style)
            // Don't reset velX and velY - let them persist
            inDash = false;
            dashFrames = 0;
            
            // Re-enable velocity clamping after dash
            velocityClampingEnabled = true;
            
            // If in air, prevent further dashing until landing
            bool onGround = (posY + height >= SCREEN_HEIGHT);
            if (!onGround && !standingOnPlatform) {
                canDash = false;  // Can't dash again until landing
            }
            
            printf("Dash ended - final velocity: (%.1f, %.1f)\n", velX, velY);
        }
    }
}

 