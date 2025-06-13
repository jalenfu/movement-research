#pragma once
#include <unordered_map>
#include <vector>
#include <set>
#include <SDL.h>
#include "input_types.hpp"
#include "controller_handler.hpp"

class InputHandler
{
private:
    std::set<Input> keyboardInputList;
    std::set<Input> controllerInputList;
    ControllerHandler controllerHandler;
    bool controllerEnabled;

public:
    InputHandler();
    ~InputHandler();
    
    void handleInput(SDL_Event& e);
    const std::set<Input>& getInputs();
    
    // Controller-specific methods
    bool initController();
    bool isControllerConnected() const;
    
    // Get analog stick values for more precise control
    double getLeftStickX() const;
    double getLeftStickY() const;
    double getRightStickX() const;
    double getRightStickY() const;
    
    // Get trigger values
    double getLeftTrigger() const;
    double getRightTrigger() const;
    
    // Enable/disable controller
    void enableController(bool enable) { controllerEnabled = enable; }
    bool isControllerEnabled() const { return controllerEnabled; }
};