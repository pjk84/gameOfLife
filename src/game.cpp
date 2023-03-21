#define _XOPEN_SOURCE_EXTENDED 1
#include "Game.hpp"
#include "Grid.hpp"
#include "Renderer.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>

using namespace GameOfLife;

Game::Game(Config config)
    : _config{config},
      _grid{Grid(config.rows, config.cols, config.width, config.height)},
      _renderer{Renderer(ceil(config.width))}
{
    _ticks = _ticksPerGeneration;
    init(config);
};

Game::~Game()
{
    std::cout << "destroyed" << std::endl;
}

void Game::render()
{
    _renderer.renderBackground();
    _renderer.renderGrid(_grid);
    _renderer.renderClear();
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        isRunning = false;
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
            isPaused = isPaused ? false : true;
            break;
        default:
            break;
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        handleMouseInput(event.button.button);
        break;
    // case SDL_MOUSEMOTION:
    //     handleMouseMotion(event.motion.x, event.motion.y);
    default:
        break;
    }
}

// void Game::handleMouseMotion(Sint32 x, Sint32 y){
//     for(auto &id: activeShapes){
//         _shapes[id].setCoords(x, y, true);
//     }
// }

void Game::handleMouseInput(uint8_t buttonIndex)
{
    int x, y;
    SDL_GetMouseState(&x, &y);
    switch (buttonIndex)
    {
    case SDL_BUTTON_LEFT:
        toggleCell(x, y);
    default:
        return;
    }
}

void Game::toggleCell(int x, int y)
{
    int cellSize = _grid.cellSize;

    if (x > _grid.marginV && x < _grid.marginV + (cellSize * _grid.cols))
    {
        if (y > _grid.marginH && y < _grid.marginH + (cellSize * _grid.rows))
        {
            int cellX = floor((x - _grid.marginV) / cellSize);
            int cellY = floor((y - _grid.marginH) / cellSize);
            return _grid.toggleCell(cellX, cellY);
        };
    };
    // click was in margin
}

void Game::clean()
{
    _renderer.tearDown();
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "game quit" << std::endl;
}

void Game::handleGeneration()
{
    _ticks--;
    if (_ticks <= 0)
    {
        _ticks = _ticksPerGeneration;
        _grid.cycleGeneration();
    }
}

void Game::init(Config config)
{
    int flags = 0;
    if (config.fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "sdl initialized..." << std::endl;
        window = SDL_CreateWindow(config.title, config.xPos, config.yPos, config.width, config.height, flags);
        if (window)
        {
            std::cout << "window created" << std::endl;
        }
        _renderer.initialize(window);
        isRunning = true;
    }
}