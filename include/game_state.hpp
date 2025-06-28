#pragma once
#include <SDL.h>

enum class GameState
{
    MENU,
    COMPLEX_PLAYER,
    SIMPLE_PLAYER
};

class GameStateManager
{
private:
    GameState currentState;
    GameState previousState; // For returning from pause

public:
    GameStateManager();
    
    void setState(GameState state);
    GameState getState() const { return currentState; }
    GameState getPreviousState() const { return previousState; }
    
    bool isInMenu() const { return currentState == GameState::MENU; }
    bool isInGameplay() const { return currentState == GameState::COMPLEX_PLAYER || currentState == GameState::SIMPLE_PLAYER; }
    
    void toggleMenu();
}; 