#include "input_handler.hpp"

void InputHandler::handleInput(SDL_Event& e)
{
    if( e.type == SDL_KEYDOWN && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_a: inputList.insert(left); break;
            case SDLK_d: inputList.insert(right); break;
            case SDLK_SPACE: inputList.insert(up); break;
            case SDLK_LSHIFT: inputList.insert(shift); break;
        }
    }
    else if( e.type == SDL_KEYUP && e.key.repeat == 0 )
    {
        switch( e.key.keysym.sym )
        {
            case SDLK_a: inputList.erase(left); break;
            case SDLK_d: inputList.erase(right); break;
            case SDLK_SPACE: inputList.erase(up); break;
            case SDLK_LSHIFT: inputList.erase(shift); break;
        }
    }
}

const std::set<Input>& InputHandler::getInputs()
{
    return inputList;
}