#include "Game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>
#include <thread>
#include <string>

int main(int argc, char *argv[])
{
    int size = 10;
    if (argc > 1)
    {
        size = std::stoi(argv[1]);
    }
    GameOfLife::Game game({"gameOfLife", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, size, false});
    while (game.isRunning)
    {
        SDL_Delay(40);
        game.handleEvents();
        game.render();
        game.handleTicks();
    }
    game.clean();

    return 1;
}