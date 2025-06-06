#pragma once
#include <unordered_map>
#include <vector>
#include <set>
#include <SDL.h>

enum Input
{
    up,
    down,
    left,
    right,
    shift
};

class InputHandler
{
private:
    std::set<Input> inputList;
public:
    void handleInput(SDL_Event& e);

    const std::set<Input>& getInputs();
};