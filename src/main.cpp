#include "Game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>
#include <thread>

int main(int argc, char *argv[])
{
    GameOfLife::Game game({"gameOfLife", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1000, 1000, 100, 100, false});
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