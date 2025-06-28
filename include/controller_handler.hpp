#pragma once
#include <SDL.h>
#include <unordered_map>
#include <set>
#include "input_types.hpp"

// Analog joystick dead zone constant
const int JOYSTICK_DEAD_ZONE = 15000;

class ControllerHandler
{
private:
    SDL_Joystick* gameController;
    bool controllerConnected;
    
    // Controller button mappings
    std::unordered_map<Uint8, Input> buttonMappings;
    
    // Track active button presses
    std::set<Uint8> activeButtons;
    
    // Analog stick values
    int leftStickX;
    int leftStickY;
    int rightStickX;
    int rightStickY;
    
    // Trigger values
    int leftTrigger;
    int rightTrigger;

public:
    ControllerHandler();
    ~ControllerHandler();
    
    // Initialize controller
    bool init();
    
    // Handle controller events
    void handleControllerEvent(SDL_Event& e);
    
    // Get processed inputs from controller
    std::set<Input> getControllerInputs();
    
    // Check if controller is connected
    bool isConnected() const { return controllerConnected; }
    
    // Get analog stick values (normalized -1.0 to 1.0)
    double getLeftStickX() const;
    double getLeftStickY() const;
    double getRightStickX() const;
    double getRightStickY() const;
    
    // Get trigger values (0.0 to 1.0)
    double getLeftTrigger() const;
    double getRightTrigger() const;
    
    // Check if start button is pressed
    bool isStartButtonPressed() const;
    
    // Check if a specific button is pressed
    bool isButtonPressed(Uint8 button) const;
}; 