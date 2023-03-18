#include "Game.hpp"
#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char *argv[])
{
    GameOfLife::Game game("gameOfLife", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 800, false);
    while (game._isRunning)
    {
        game.handleEvents();
        game.render();
    }
    game.clean();

    return 1;
}