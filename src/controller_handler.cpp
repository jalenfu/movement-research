#include "controller_handler.hpp"
#include <cmath>

ControllerHandler::ControllerHandler()
{
    gameController = nullptr;
    controllerConnected = false;
    
    // Initialize analog stick values
    leftStickX = 0;
    leftStickY = 0;
    rightStickX = 0;
    rightStickY = 0;
    
    // Initialize trigger values
    leftTrigger = 0;
    rightTrigger = 0;
    
    // Set up button mappings (common controller layout)
    buttonMappings = {
        {0, up},      // A button (Xbox) / Cross (PlayStation)
        {1, shift},   // B button (Xbox) / Circle (PlayStation)
        {2, down},    // X button (Xbox) / Square (PlayStation)
        {3, right},   // Y button (Xbox) / Triangle (PlayStation)
        {4, left},    // Left bumper
        {5, right},   // Right bumper
        {6, shift},   // Back button
        {7, up},      // Start button
        {8, left},    // Left stick button
        {9, right},   // Right stick button
        {10, up},     // Left trigger (as button)
        {11, down},   // Right trigger (as button)
        {12, up},     // D-pad up
        {13, down},   // D-pad down
        {14, left},   // D-pad left
        {15, right}   // D-pad right
    };
}

ControllerHandler::~ControllerHandler()
{
    if (gameController != nullptr)
    {
        SDL_JoystickClose(gameController);
        gameController = nullptr;
    }
}

bool ControllerHandler::init()
{
    // Check for joysticks
    if (SDL_NumJoysticks() < 1)
    {
        printf("Warning: No joysticks connected!\n");
        controllerConnected = false;
        return false;
    }
    
    // Load joystick
    gameController = SDL_JoystickOpen(0);
    if (gameController == nullptr)
    {
        printf("Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError());
        controllerConnected = false;
        return false;
    }
    
    printf("Controller connected: %s\n", SDL_JoystickName(gameController));
    printf("Controller has %d axes, %d buttons, %d hats\n", 
           SDL_JoystickNumAxes(gameController),
           SDL_JoystickNumButtons(gameController),
           SDL_JoystickNumHats(gameController));
    
    controllerConnected = true;
    return true;
}

void ControllerHandler::handleControllerEvent(SDL_Event& e)
{
    if (!controllerConnected) return;
    
    switch (e.type)
    {
        case SDL_JOYAXISMOTION:
            // Motion on controller 0
            if (e.jaxis.which == 0)
            {
                switch (e.jaxis.axis)
                {
                    case 0: // Left stick X
                        leftStickX = e.jaxis.value;
                        break;
                    case 1: // Left stick Y
                        leftStickY = e.jaxis.value;
                        break;
                    case 2: // Right stick X
                        rightStickX = e.jaxis.value;
                        break;
                    case 3: // Right stick Y
                        rightStickY = e.jaxis.value;
                        break;
                    case 4: // Left trigger
                        leftTrigger = e.jaxis.value;
                        break;
                    case 5: // Right trigger
                        rightTrigger = e.jaxis.value;
                        break;
                }
            }
            break;
            
        case SDL_JOYBUTTONDOWN:
            // Button press on controller 0
            if (e.jbutton.which == 0)
            {
                // Handle button press if mapped
                auto it = buttonMappings.find(e.jbutton.button);
                if (it != buttonMappings.end())
                {
                    // Add button to active inputs
                    activeButtons.insert(e.jbutton.button);
                }
            }
            break;
            
        case SDL_JOYBUTTONUP:
            // Button release on controller 0
            if (e.jbutton.which == 0)
            {
                // Remove button from active inputs
                activeButtons.erase(e.jbutton.button);
            }
            break;
            
        case SDL_JOYHATMOTION:
            // Hat motion on controller 0
            if (e.jhat.which == 0)
            {
                // Handle D-pad input
                // This is handled through the analog stick processing
            }
            break;
    }
}

std::set<Input> ControllerHandler::getControllerInputs()
{
    std::set<Input> inputs;
    
    if (!controllerConnected) return inputs;
    
    // Process left stick for movement
    if (leftStickX < -JOYSTICK_DEAD_ZONE)
    {
        inputs.insert(left);
    }
    else if (leftStickX > JOYSTICK_DEAD_ZONE)
    {
        inputs.insert(right);
    }
    
    // Make analog up direction less sensitive for jumping
    // Use a higher threshold for up movement
    if (leftStickY < -JOYSTICK_DEAD_ZONE * 1.5)  // 1.5x more sensitive threshold
    {
        inputs.insert(up);
    }
    else if (leftStickY > JOYSTICK_DEAD_ZONE)
    {
        inputs.insert(down);
    }
    
    // Process button presses
    for (Uint8 button : activeButtons)
    {
        auto it = buttonMappings.find(button);
        if (it != buttonMappings.end())
        {
            inputs.insert(it->second);
        }
    }
    
    // Process triggers
    if (leftTrigger > JOYSTICK_DEAD_ZONE || rightTrigger > JOYSTICK_DEAD_ZONE)
    {
        inputs.insert(shift);
    }
    
    return inputs;
}

double ControllerHandler::getLeftStickX() const
{
    if (!controllerConnected) return 0.0;
    
    if (abs(leftStickX) < JOYSTICK_DEAD_ZONE)
        return 0.0;
    
    return static_cast<double>(leftStickX) / 32767.0;
}

double ControllerHandler::getLeftStickY() const
{
    if (!controllerConnected) return 0.0;
    
    if (abs(leftStickY) < JOYSTICK_DEAD_ZONE)
        return 0.0;
    
    return static_cast<double>(leftStickY) / 32767.0;
}

double ControllerHandler::getRightStickX() const
{
    if (!controllerConnected) return 0.0;
    
    if (abs(rightStickX) < JOYSTICK_DEAD_ZONE)
        return 0.0;
    
    return static_cast<double>(rightStickX) / 32767.0;
}

double ControllerHandler::getRightStickY() const
{
    if (!controllerConnected) return 0.0;
    
    if (abs(rightStickY) < JOYSTICK_DEAD_ZONE)
        return 0.0;
    
    return static_cast<double>(rightStickY) / 32767.0;
}

double ControllerHandler::getLeftTrigger() const
{
    if (!controllerConnected) return 0.0;
    
    if (leftTrigger < JOYSTICK_DEAD_ZONE)
        return 0.0;
    
    return static_cast<double>(leftTrigger) / 32767.0;
}

double ControllerHandler::getRightTrigger() const
{
    if (!controllerConnected) return 0.0;
    
    if (rightTrigger < JOYSTICK_DEAD_ZONE)
        return 0.0;
    
    return static_cast<double>(rightTrigger) / 32767.0;
} 