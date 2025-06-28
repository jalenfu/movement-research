#include "input_handler.hpp"

InputHandler::InputHandler()
{
    controllerEnabled = true;
}

InputHandler::~InputHandler()
{
    // Controller handler destructor will handle cleanup
}

void InputHandler::handleInput(SDL_Event& e)
{
    // Handle keyboard input
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_a: keyboardInputList.insert(left); break;
            case SDLK_d: keyboardInputList.insert(right); break;
            case SDLK_s: keyboardInputList.insert(down); break;
            case SDLK_SPACE: keyboardInputList.insert(up); break;
            case SDLK_LSHIFT: keyboardInputList.insert(shift); break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_a: keyboardInputList.erase(left); break;
            case SDLK_d: keyboardInputList.erase(right); break;
            case SDLK_s: keyboardInputList.erase(down); break;
            case SDLK_SPACE: keyboardInputList.erase(up); break;
            case SDLK_LSHIFT: keyboardInputList.erase(shift); break;
        }
    }
    
    // Handle controller input if enabled
    if (controllerEnabled)
    {
        controllerHandler.handleControllerEvent(e);
        // Update controller input list
        controllerInputList = controllerHandler.getControllerInputs();
    }
}

const std::set<Input>& InputHandler::getInputs()
{
    // Create a temporary set to combine keyboard and controller inputs
    static std::set<Input> combinedInputs;
    combinedInputs.clear();
    
    // Add keyboard inputs
    for (Input input : keyboardInputList)
    {
        combinedInputs.insert(input);
    }
    
    // Add controller inputs if enabled and connected
    if (controllerEnabled && controllerHandler.isConnected())
    {
        for (Input input : controllerInputList)
        {
            combinedInputs.insert(input);
        }
    }
    
    return combinedInputs;
}

bool InputHandler::initController()
{
    if (controllerEnabled)
    {
        return controllerHandler.init();
    }
    return false;
}

bool InputHandler::isControllerConnected() const
{
    return controllerEnabled && controllerHandler.isConnected();
}

double InputHandler::getLeftStickX() const
{
    if (controllerEnabled && controllerHandler.isConnected())
    {
        return controllerHandler.getLeftStickX();
    }
    return 0.0;
}

double InputHandler::getLeftStickY() const
{
    if (controllerEnabled && controllerHandler.isConnected())
    {
        return controllerHandler.getLeftStickY();
    }
    return 0.0;
}

double InputHandler::getRightStickX() const
{
    if (controllerEnabled && controllerHandler.isConnected())
    {
        return controllerHandler.getRightStickX();
    }
    return 0.0;
}

double InputHandler::getRightStickY() const
{
    if (controllerEnabled && controllerHandler.isConnected())
    {
        return controllerHandler.getRightStickY();
    }
    return 0.0;
}

double InputHandler::getLeftTrigger() const
{
    if (controllerEnabled && controllerHandler.isConnected())
    {
        return controllerHandler.getLeftTrigger();
    }
    return 0.0;
}

double InputHandler::getRightTrigger() const
{
    if (controllerEnabled && controllerHandler.isConnected())
    {
        return controllerHandler.getRightTrigger();
    }
    return 0.0;
}

bool InputHandler::isPausePressed() const
{
    if (controllerEnabled && controllerHandler.isConnected())
    {
        return controllerHandler.isStartButtonPressed();
    }
    return false;
}

bool InputHandler::isKeyPressed(SDL_Keycode key) const
{
    const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);
    SDL_Scancode scancode = SDL_GetScancodeFromKey(key);
    return currentKeyStates[scancode];
}