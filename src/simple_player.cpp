#include "simple_player.hpp"
#include "library.hpp"
#include <cmath>

SimplePlayer::SimplePlayer()
{
    posX = 0;
    posY = 300;
    lastPosX = posX;
    lastPosY = posY;

    gravity = 0.6;
    moveSpeed = 8.0;        // Constant horizontal speed
    jumpSpeed = -12.0;      // Initial jump velocity
    doubleJumpSpeed = -8.0; // Double jump velocity (shorter than regular jump)
    dashSpeed = 16.0;       // Dash velocity (reduced from 20.0)
    dashDuration = 8;       // 8 frames of dash (extended from 4)
    dashCooldown = 15;      // 15 frame cooldown between dashes
    
    // Wall jump
    wallJumpSpeedX = 6.0;   // Horizontal wall jump velocity (half as much)
    wallJumpSpeedY = -12.0; // Vertical wall jump velocity (slightly more)
    canWallJump = false;
    touchingWall = false;
    touchingLeftWall = false;
    touchingRightWall = false;
    lastTouchingLeftWall = false;
    lastTouchingRightWall = false;
    facingRight = true;
    inWallJump = false;
    wallJumpFrames = 0;
    wallJumpControlDelay = 8; // 8 frames before regaining control
    wallJumpBufferFrames = 5; // 5 frames buffer after leaving wall
    wallJumpBufferTimer = 0;
    
    velX = 0;
    velY = 0;
    width = 20;
    height = 20;

    onGround = false;
    onPlatform = false;
    standingOnPlatform = false;
    inDash = false;
    canDoubleJump = false;  // Start without double jump
    dashFrames = 0;
    dashCooldownFrames = 0;
    wasJumpPressed = false;
    wasDashPressed = false;
    
    texture.loadFromFile( "src/dot.bmp" );
}

SimplePlayer::~SimplePlayer()
{
    texture.free();
}

bool SimplePlayer::canWallJumpNow() const
{
    return (touchingWall || wallJumpBufferTimer > 0) && canWallJump;
}

void SimplePlayer::handleEvent(InputHandler& inputHandler)
{
    // Reset horizontal velocity if not dashing and not in wall jump control delay
    if (!inDash && (!inWallJump || wallJumpFrames >= wallJumpControlDelay)) {
        velX = 0;
    }
    
    // Handle digital inputs
	for (Input i : inputHandler.getInputs()) {
        switch (i)
        {
        case left:
            if (!inDash && (!inWallJump || wallJumpFrames >= wallJumpControlDelay)) {
                velX = -moveSpeed;  // Constant velocity
                facingRight = false;
            }
            break;
        case right:
            if (!inDash && (!inWallJump || wallJumpFrames >= wallJumpControlDelay)) {
                velX = moveSpeed;   // Constant velocity
                facingRight = true;
            }
            break;
        case up:
            // Jump - ground/platform jump, wall jump, or double jump
            if (!wasJumpPressed) {
                bool canJumpFromGround = (posY + height >= SCREEN_HEIGHT) || standingOnPlatform;
                if (canJumpFromGround) {
                    velY = jumpSpeed;
                    canWallJump = true;  // Enable wall jump after ground jump
                    canDoubleJump = true; // Enable double jump after ground jump
                    printf("Jump from ground/platform!\n");
                } else if (canWallJumpNow()) {
                    // Wall jump - push up and away from wall
                    if (touchingLeftWall || (wallJumpBufferTimer > 0 && lastTouchingLeftWall)) {
                        velX = wallJumpSpeedX;  // Push right (away from left wall)
                        printf("Wall jump from left wall! Velocity: (%.1f, %.1f)\n", velX, wallJumpSpeedY);
                    } else if (touchingRightWall || (wallJumpBufferTimer > 0 && lastTouchingRightWall)) {
                        velX = -wallJumpSpeedX; // Push left (away from right wall)
                        printf("Wall jump from right wall! Velocity: (%.1f, %.1f)\n", velX, wallJumpSpeedY);
                    }
                    velY = wallJumpSpeedY;
                    canWallJump = false;  // Use up wall jump
                    canDoubleJump = true; // Refresh double jump after wall jump
                    wallJumpBufferTimer = 0; // Clear buffer
                    inWallJump = true;
                    wallJumpFrames = 0;
                } else if (canDoubleJump) {
                    velY = doubleJumpSpeed;
                    canDoubleJump = false; // Use up double jump
                    printf("Double jump!\n");
                }
            }
            wasJumpPressed = true;
            break;
        case down:
            // Fast fall - increase gravity when falling
            if (velY > 0) {
                velY += 2.0;
            }
            break;
        case shift:
            // Dash - brief, fixed duration
            if (!wasDashPressed && !inDash && dashCooldownFrames <= 0) {
                inDash = true;
                dashFrames = 0;
                dashCooldownFrames = dashCooldown;
                
                // Dash in facing direction
                velX = facingRight ? dashSpeed : -dashSpeed;
                velY = 0;  // Reset vertical velocity during dash
                printf("Dash!\n");
            }
            wasDashPressed = true;
            break;
        default:
            break;
        }
    }
    
    // Reset button press states
    if (!inputHandler.getInputs().count(up)) {
        wasJumpPressed = false;
    }
    if (!inputHandler.getInputs().count(shift)) {
        wasDashPressed = false;
    }
    
    // Handle analog input
    if (inputHandler.isControllerConnected())
    {
        double leftStickX = inputHandler.getLeftStickX();
        double leftStickY = inputHandler.getLeftStickY();
        
        // Apply analog movement if no digital input and not dashing and not in wall jump control delay
        if (!inDash && (!inWallJump || wallJumpFrames >= wallJumpControlDelay) && std::fabs(leftStickX) > 0.1)
        {
            velX = leftStickX * moveSpeed;
            facingRight = (leftStickX > 0);
        }
        // If no analog input and no digital input, velX stays 0 (already reset above)
        
        // Apply analog jump
        if (leftStickY < -0.5 && !wasJumpPressed) {
            bool canJumpFromGround = (posY + height >= SCREEN_HEIGHT) || standingOnPlatform;
            if (canJumpFromGround) {
                velY = jumpSpeed;
                canWallJump = true;
                canDoubleJump = true; // Enable double jump after ground jump
                printf("Analog jump from ground/platform!\n");
            } else if (canWallJumpNow()) {
                if (touchingLeftWall || (wallJumpBufferTimer > 0 && lastTouchingLeftWall)) {
                    velX = wallJumpSpeedX;  // Push right (away from left wall)
                    printf("Analog wall jump from left wall! Velocity: (%.1f, %.1f)\n", velX, wallJumpSpeedY);
                } else if (touchingRightWall || (wallJumpBufferTimer > 0 && lastTouchingRightWall)) {
                    velX = -wallJumpSpeedX; // Push left (away from right wall)
                    printf("Analog wall jump from right wall! Velocity: (%.1f, %.1f)\n", velX, wallJumpSpeedY);
                }
                velY = wallJumpSpeedY;
                canWallJump = false;
                canDoubleJump = true; // Refresh double jump after wall jump
                wallJumpBufferTimer = 0; // Clear buffer
                inWallJump = true;
                wallJumpFrames = 0;
            } else if (canDoubleJump) {
                velY = doubleJumpSpeed;
                canDoubleJump = false; // Use up double jump
                printf("Analog double jump!\n");
            }
            wasJumpPressed = true;
        }
    }
}

void SimplePlayer::update(InputHandler& inputHandler)
{
    // Update dash state
    if (inDash) {
        dashFrames++;
        if (dashFrames >= dashDuration) {
            inDash = false;
            velX = 0;  // Stop horizontal movement after dash
            printf("Dash ended\n");
        }
    }
    
    // Update dash cooldown
    if (dashCooldownFrames > 0) {
        dashCooldownFrames--;
    }
    
    // Update wall jump state
    if (inWallJump) {
        wallJumpFrames++;
        if (wallJumpFrames >= wallJumpControlDelay) {
            inWallJump = false;
            printf("Wall jump control regained\n");
        }
    }
    
    // Update wall jump buffer timer
    if (wallJumpBufferTimer > 0) {
        wallJumpBufferTimer--;
        if (wallJumpBufferTimer == 0) {
            printf("Wall jump buffer expired\n");
        }
    }
    
    // Update position
    posX += velX;
    posY += velY;

    // Handle horizontal boundary collisions (walls)
    bool wasTouchingWall = touchingWall;
    touchingLeftWall = false;
    touchingRightWall = false;
    
    if (posX < 0)
    {
        posX = 0;
        velX = 0;
        touchingWall = true;
        touchingLeftWall = true;
    }
    else if (posX + width > SCREEN_WIDTH)
    {
        posX = SCREEN_WIDTH - width;
        velX = 0;
        touchingWall = true;
        touchingRightWall = true;
    }
    else
    {
        touchingWall = false;
    }
    
    // Manage wall jump buffer and refresh
    if (touchingWall && !wasTouchingWall) {
        // Just touched wall - refresh wall jump and clear buffer
        canWallJump = true;
        wallJumpBufferTimer = 0;
        // Remember which wall was touched
        lastTouchingLeftWall = touchingLeftWall;
        lastTouchingRightWall = touchingRightWall;
        printf("Wall jump refreshed!\n");
    } else if (!touchingWall && wasTouchingWall) {
        // Just left wall - start buffer timer
        wallJumpBufferTimer = wallJumpBufferFrames;
        printf("Wall jump buffer started (%d frames)\n", wallJumpBufferTimer);
    }

    // Handle vertical boundary collisions
    if (posY < 0)
    {
        posY = 0;
        velY = 0;
    }
    else if (posY + height > SCREEN_HEIGHT)
    {
        posY = SCREEN_HEIGHT - height;
        velY = 0;
        onGround = true;
        canWallJump = true;  // Reset wall jump on ground
    }

    // Check platform collisions with pass-through logic
    bool holdingDown = false;
    
    // Check digital inputs for down
    for (Input i : inputHandler.getInputs()) {
        if (i == down) {
            holdingDown = true;
            break;
        }
    }
    
    // Check analog input for down
    if (inputHandler.isControllerConnected() && inputHandler.getLeftStickY() > 0.5) {
        holdingDown = true;
    }
    
    // Use the same platform collision system as ComplexPlayer
    onPlatform = PlatformManager::checkPlatformCollisionWithPrevious(posX, posY, lastPosX, lastPosY, width, height, velY, holdingDown);
    standingOnPlatform = PlatformManager::isOnPlatform(posX, posY, width, height, velY, holdingDown);
    
    // Handle platform landing - place player exactly on platform
    if (onPlatform && velY > 0) // Landing on platform
    {
        // Find the platform we're landing on and place player on it
        for (const Platform& platform : PlatformManager::getPlatforms())
        {
            if (posX + width > platform.x && 
                posX < platform.x + platform.width)
            {
                // Check if we're currently on the platform surface
                if (posY + height >= platform.y && 
                    posY + height <= platform.y + 2)
                {
                    posY = platform.y - height; // Place exactly on platform
                    break;
                }
                // Check if we tunneled through the platform
                else if (lastPosY + height < platform.y && 
                         posY + height > platform.y)
                {
                    posY = platform.y - height; // Place exactly on platform
                    break;
                }
            }
        }
        velY = 0; // Stop downward movement
        canWallJump = true;  // Reset wall jump on platform
    }

    // Check if on ground (at bottom of screen) - AFTER position update
    bool onGround = (posY + height >= SCREEN_HEIGHT);

    // Only apply gravity if not on ground or platform
    if (!onGround && !standingOnPlatform)
    {
        velY += gravity;
    }

    // Check target collisions
    handleTargetCollision();
    
    // Update last position for next frame
    lastPosX = posX;
    lastPosY = posY;
}

bool SimplePlayer::checkPlatformCollision()
{
    return onPlatform;
}

bool SimplePlayer::isOnGroundOrPlatform() const
{
    return onGround || onPlatform;
}

bool SimplePlayer::checkTargetCollision()
{
    return TargetManager::checkTargetCollision(posX, posY, width, height);
}

void SimplePlayer::handleTargetCollision()
{
    TargetManager::handleTargetCollision(posX, posY, width, height);
}

void SimplePlayer::render()
{
    texture.render(posX, posY);
} 