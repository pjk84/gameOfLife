#include "Game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>
#include <thread>

int main(int argc, char *argv[])
{
    GameOfLife::Game game({"gameOfLife", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 30, false});
    while (game.isRunning)
    {
        game.handleEvents();
        if (!game.isPaused)
        {
            game.handleGeneration();
        }
        game.render();
    }
    game.clean();

    return 1;
}