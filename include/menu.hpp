#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include "game_state.hpp"

// Forward declaration
class InputHandler;

class Menu
{
private:
    int selectedOption;
    int numOptions;
    bool keyPressed;
    TTF_Font* font;
    
    // Menu options
    static const int MENU_OPTIONS = 2;
    const char* menuItems[MENU_OPTIONS] = {
        "Complex Movement (Celeste-style)",
        "Simple Movement (Hollow Knight-style)"
    };

public:
    Menu();
    ~Menu();
    
    void handleInput(SDL_Event& e, GameStateManager& gameState);
    void handleControllerInput(InputHandler& inputHandler, GameStateManager& gameState);
    void render();
    void reset();
    
    int getSelectedOption() const { return selectedOption; }
}; 