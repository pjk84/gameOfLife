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
#include <iomanip>
#include <cmath>
#include <complex>
#include <tuple>

using namespace GameOfLife;

typedef std::complex<double> point;
#define xCoord x()
#define yCoord y()

// Constant PI for providing angles in radians
#define PI 3.1415926535897932384626

Game::Game(Config config)
    : _config{config},
      _grid{Grid(config.gridSize, config.width, config.height)},
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

    std::tuple<int, int> mouseCoords = getCellCoordinates(mouseX, mouseY);

    if (isoMetric)
    {
        // pass mouse coords
        _renderer.renderGridIsometric(_grid, mouseCoords);
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
    case SDL_MOUSEMOTION:
        handleMouseMotionEvent(event.motion);
        break;
    case SDL_MOUSEBUTTONDOWN:
        handleMouseButtonDown(event.button.button);
        break;
    // case SDL_MOUSEMOTION:
    //     handleMouseMotion(event.motion.x, event.motion.y);
    default:
        break;
    }
}

void Game::handleMouseMotionEvent(SDL_MouseMotionEvent event)
{
    mouseX = event.x;
    mouseY = event.y;
}

void Game::handleMouseButtonDown(uint8_t buttonIndex)
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
    std::tuple<int, int> coords = getCellCoordinates(x, y);
    int cellX = std::get<0>(coords);
    int cellY = std::get<1>(coords);
    if (cellX >= 0 && cellX < _grid.size)
    {
        if (cellY >= 0 && cellY < _grid.size)
        {
            // toggle cell if if window coords are within grid bounds
            return _grid.toggleCell(cellX, cellY);
        };
    };
}

std::tuple<int, int> Game::getCellCoordinates(int x, int y)
{
    int cellSize = _grid.cellSize;
    int cellY, cellX;
    int marginX = _grid.marginX;
    int marginY = _grid.marginY;
    if (isoMetric)
    {
        // transform to rectangle to find grid coords

        // get origin coords
        int xCenter = _config.width / 2;
        int yCenter = _config.height / 2;

        // get mouse coords relative to origin
        long a = x - xCenter;
        long b = (yCenter - y) * 2;

        // calculate new cell width after rotation
        double newCellWidth = sqrt(pow(cellSize / 2, 2) + pow(cellSize / 2, 2));

        // anti-clockwise rotation of 315deg to get a clock-wise rotation of 45deg
        point p(a, b);
        point n = p * std::polar(1.0, 1.75 * PI);

        // set new margin after rotation
        marginX = (_config.width - (newCellWidth * _grid.size)) / 2;
        marginY = marginX;

        cellX = ((n.real() + xCenter) - marginX) / newCellWidth;
        cellY = ((yCenter - n.imag()) - marginX) / newCellWidth;
    }
    else
    {
        cellX = floor((x - _grid.marginX) / cellSize);
        cellY = floor((y - _grid.marginY) / cellSize);
    }

    return std::make_tuple(cellX, cellY);
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