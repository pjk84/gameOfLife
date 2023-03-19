#include "Game.hpp"
#include <iostream>
#include <SDL2/SDL.h>
#include <chrono>
#include <thread>

int main(int argc, char *argv[])
{
    GameOfLife::Game game({"gameOfLife", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 800, 15, false});
    while (game.isRunning)
    {
        game.handleEvents();
        game.handleGeneration();
        game.render();
        // std::this_thread::sleep_for(std::chrono::milliseconds(10000000));
    }
    game.clean();

    return 1;
}