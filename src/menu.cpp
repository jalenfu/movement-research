#include "menu.hpp"
#include "library.hpp"
#include "input_handler.hpp"
#include <iostream>

Menu::Menu()
{
    selectedOption = 0;
    numOptions = MENU_OPTIONS;
    keyPressed = false;
    
    // Load default font
    font = TTF_OpenFont("arial.ttf", 24);
    if (font == nullptr) {
        // Try to use system font if arial.ttf is not available
        font = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 24);
    }
    if (font == nullptr) {
        printf("Warning: Could not load font! SDL_ttf Error: %s\n", TTF_GetError());
    }
}

Menu::~Menu()
{
    if (font != nullptr) {
        TTF_CloseFont(font);
        font = nullptr;
    }
}

void Menu::handleInput(SDL_Event& e, GameStateManager& gameState)
{
    if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            if (!keyPressed) {
                selectedOption = (selectedOption - 1 + numOptions) % numOptions;
                keyPressed = true;
            }
            break;
        case SDLK_DOWN:
            if (!keyPressed) {
                selectedOption = (selectedOption + 1) % numOptions;
                keyPressed = true;
            }
            break;
        case SDLK_RETURN:
        case SDLK_SPACE:
            if (!keyPressed) {
                // Select the current option
                if (selectedOption == 0) {
                    gameState.setState(GameState::COMPLEX_PLAYER);
                } else if (selectedOption == 1) {
                    gameState.setState(GameState::SIMPLE_PLAYER);
                }
                keyPressed = true;
            }
            break;
        case SDLK_ESCAPE:
            if (!keyPressed) {
                gameState.toggleMenu();
                keyPressed = true;
            }
            break;
        default:
            break;
        }
    }
    else if (e.type == SDL_KEYUP)
    {
        keyPressed = false;
    }
}

void Menu::handleControllerInput(InputHandler& inputHandler, GameStateManager& gameState)
{
    static bool controllerKeyPressed = false;
    static int controllerRepeatDelay = 0;
    const int REPEAT_DELAY = 15; // Frames to wait before repeating input
    
    if (!inputHandler.isControllerConnected()) {
        return;
    }
    
    // Get direct controller access for menu-specific input handling
    const ControllerHandler& controller = inputHandler.getControllerHandler();
    
    // Handle D-pad or left stick for navigation (using raw stick values)
    double leftStickY = controller.getLeftStickY();
    bool upPressed = leftStickY < -0.5;  // Threshold for up movement
    bool downPressed = leftStickY > 0.5; // Threshold for down movement
    
    // Check for A button (button 0) press directly
    bool selectPressed = controller.isButtonPressed(0);
    
    // Handle navigation with repeat delay
    if (upPressed || downPressed) {
        if (!controllerKeyPressed || controllerRepeatDelay <= 0) {
            if (upPressed) {
                selectedOption = (selectedOption - 1 + numOptions) % numOptions;
            } else if (downPressed) {
                selectedOption = (selectedOption + 1) % numOptions;
            }
            controllerKeyPressed = true;
            controllerRepeatDelay = REPEAT_DELAY;
        }
    } else {
        controllerKeyPressed = false;
        controllerRepeatDelay = 0;
    }
    
    // Handle selection (only trigger once per press)
    static bool lastSelectPressed = false;
    if (selectPressed && !lastSelectPressed) {
        if (selectedOption == 0) {
            gameState.setState(GameState::COMPLEX_PLAYER);
        } else if (selectedOption == 1) {
            gameState.setState(GameState::SIMPLE_PLAYER);
        }
    }
    lastSelectPressed = selectPressed;
    
    // Handle pause button
    if (inputHandler.isPausePressed()) {
        gameState.toggleMenu();
    }
    
    // Decrease repeat delay
    if (controllerRepeatDelay > 0) {
        controllerRepeatDelay--;
    }
}

void Menu::render()
{
    // Clear screen
    SDL_SetRenderDrawColor(gRenderer, 0x20, 0x20, 0x40, 0xFF);
    SDL_RenderClear(gRenderer);
    
    if (font == nullptr) {
        // Fallback to rectangle rendering if font is not available
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_Rect titleRect = {SCREEN_WIDTH / 2 - 150, 100, 300, 50};
        SDL_RenderFillRect(gRenderer, &titleRect);
        
        for (int i = 0; i < numOptions; i++)
        {
            SDL_Rect optionRect = {SCREEN_WIDTH / 2 - 200, 200 + i * 80, 400, 60};
            
            if (i == selectedOption) {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
            } else {
                SDL_SetRenderDrawColor(gRenderer, 0x80, 0x80, 0x80, 0xFF);
            }
            
            SDL_RenderFillRect(gRenderer, &optionRect);
        }
        
        SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
        SDL_Rect instructionRect = {SCREEN_WIDTH / 2 - 200, SCREEN_HEIGHT - 100, 400, 30};
        SDL_RenderFillRect(gRenderer, &instructionRect);
    } else {
        // Render title
        SDL_Color titleColor = {255, 255, 255, 255};
        SDL_Surface* titleSurface = TTF_RenderText_Solid(font, "Movement Research", titleColor);
        if (titleSurface != nullptr) {
            SDL_Texture* titleTexture = SDL_CreateTextureFromSurface(gRenderer, titleSurface);
            if (titleTexture != nullptr) {
                SDL_Rect titleRect = {SCREEN_WIDTH / 2 - titleSurface->w / 2, 100, titleSurface->w, titleSurface->h};
                SDL_RenderCopy(gRenderer, titleTexture, NULL, &titleRect);
                SDL_DestroyTexture(titleTexture);
            }
            SDL_FreeSurface(titleSurface);
        }
        
        // Render menu options
        for (int i = 0; i < numOptions; i++)
        {
            SDL_Color textColor;
            if (i == selectedOption) {
                textColor = {255, 255, 0, 255}; // Yellow for selected
            } else {
                textColor = {200, 200, 200, 255}; // Gray for unselected
            }
            
            SDL_Surface* textSurface = TTF_RenderText_Solid(font, menuItems[i], textColor);
            if (textSurface != nullptr) {
                SDL_Texture* textTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
                if (textTexture != nullptr) {
                    SDL_Rect textRect = {SCREEN_WIDTH / 2 - textSurface->w / 2, 200 + i * 80, textSurface->w, textSurface->h};
                    SDL_RenderCopy(gRenderer, textTexture, NULL, &textRect);
                    SDL_DestroyTexture(textTexture);
                }
                SDL_FreeSurface(textSurface);
            }
        }
        
        // Render instructions
        SDL_Color instructionColor = {255, 255, 255, 255};
        const char* instructions = "Use UP/DOWN or D-pad to navigate, ENTER/SPACE/A to select, ESC/START to pause";
        SDL_Surface* instructionSurface = TTF_RenderText_Solid(font, instructions, instructionColor);
        if (instructionSurface != nullptr) {
            SDL_Texture* instructionTexture = SDL_CreateTextureFromSurface(gRenderer, instructionSurface);
            if (instructionTexture != nullptr) {
                SDL_Rect instructionRect = {SCREEN_WIDTH / 2 - instructionSurface->w / 2, SCREEN_HEIGHT - 100, instructionSurface->w, instructionSurface->h};
                SDL_RenderCopy(gRenderer, instructionTexture, NULL, &instructionRect);
                SDL_DestroyTexture(instructionTexture);
            }
            SDL_FreeSurface(instructionSurface);
        }
    }
    
    // Update screen
    SDL_RenderPresent(gRenderer);
}

void Menu::reset()
{
    selectedOption = 0;
    keyPressed = false;
} 