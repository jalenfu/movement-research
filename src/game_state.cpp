#include "game_state.hpp"

GameStateManager::GameStateManager()
{
    currentState = GameState::MENU;
    previousState = GameState::MENU;
}

void GameStateManager::setState(GameState state)
{
    previousState = currentState;
    currentState = state;
}

void GameStateManager::toggleMenu()
{
    if (isInGameplay()) {
        // Save current state and go to menu
        previousState = currentState;
        currentState = GameState::MENU;
    } else {
        // Return to previous gameplay state
        currentState = previousState;
    }
} 