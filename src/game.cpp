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
    // _renderer.renderGridFlat(_grid);
    if (isoMetric)
    {

        _renderer.renderGridIsometric(_grid);
    }
    else
    {
        _renderer.renderGridFlat(_grid);
    }
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
        case SDLK_SPACE:
            isoMetric = !isoMetric;
            break;
        case SDLK_p:
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

    if (x > _grid.marginX && x < _grid.marginX + (cellSize * _grid.cols))
    {
        if (y > _grid.marginY && y < _grid.marginY + (cellSize * _grid.rows))
        {
            int cellX = floor((x - _grid.marginX) / cellSize);
            int cellY = floor((y - _grid.marginY) / cellSize);
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

void Game::handleTicks()
{
    _ticks--;
    if (_ticks <= 0)
    {
        if (!isPaused)
        {
            _grid.cycleGeneration();
        }
        _ticks = _ticksPerGeneration;
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