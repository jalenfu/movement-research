#include "keyboard_handler.hpp"

void KeyboardHandler::handleKeyboardEvent(SDL_Event& e)
{
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_a: keyPressed[SDLK_a] = true; break;
            case SDLK_d: keyPressed[SDLK_d] = true; break;
            case SDLK_SPACE: keyPressed[SDLK_SPACE] = true; break;
            case SDLK_LSHIFT: keyPressed[SDLK_LSHIFT] = true; break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_a: keyPressed[SDLK_a] = false; break;
            case SDLK_d: keyPressed[SDLK_d] = false; break;
            case SDLK_SPACE: keyPressed[SDLK_SPACE] = false; break;
            case SDLK_LSHIFT: keyPressed[SDLK_LSHIFT] = false; break;
        }
    }
}

bool KeyboardHandler::isPressed(SDL_KeyCode k)
{
    return keyPressed[k];
}