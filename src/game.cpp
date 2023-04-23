#define _XOPEN_SOURCE_EXTENDED 1
#include "Game.hpp"
#include "Grid.hpp"
#include "Renderer.hpp"
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <math.h>
#include <iomanip>
#include <cmath>
#include <string>
#include <complex>
#include <tuple>
#include <array>
#include <filesystem>

using namespace GameOfLife;

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

void Game::render(int ticks)
{

    _renderer.renderBackground();

    if (isoMetric)
    {
        // pass mouse coords
        auto mouseCoords = getCellCoordinates(mouseProps.x, mouseProps.y);
        _renderer.renderGridIsometric(_grid, mouseCoords, ticks);
        renderText();
    }
    else
    {
        _renderer.renderGridFlat(_grid);
    }
    _renderer.renderClear();
}

void Game::renderText()
{
    // render text
    std::string stats = "generation: " + std::to_string(_grid.generation);
    stats += " - population: " + std::to_string(_grid.population);
    if (isPaused)
    {
        stats += " - paused";
    }
    _renderer.renderText({25, 10, stats.c_str(), {255, 255, 255}, font});
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
        case SDLK_r:
            _grid.reset();
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
    case SDL_MOUSEBUTTONUP:
        handleMouseButtonUp();
        break;
    // case SDL_MOUSEMOTION:
    //     handleMouseMotion(event.motion.x, event.motion.y);
    default:
        break;
    }
}

void Game::handleMouseMotionEvent(SDL_MouseMotionEvent event)
{
    mouseProps.x = event.x;
    mouseProps.y = event.y;
    if (mouseProps.isDown)
    {
        Coords coords = getCellCoordinates(mouseProps.x, mouseProps.y);
        for (const auto &v : mouseProps.cellsToggled)
        {
            if (v[0] == coords[0] && v[1] == coords[1])
            {
                // cell already toggled since mousedown
                return;
            }
        }
        toggleCell();
    }
}

void Game::handleMouseButtonDown(uint8_t buttonIndex)
{
    // int x, y;
    // SDL_GetMouseState(&x, &y);
    switch (buttonIndex)
    {
    case SDL_BUTTON_LEFT:
        mouseProps.isDown = true;
        toggleCell();
        break;
    default:
        return;
    }
}

void Game::handleMouseButtonUp()
{

    mouseProps.cellsToggled.clear();
    mouseProps.isDown = false;
}

void Game::toggleCell()
{
    Coords coords = getCellCoordinates(mouseProps.x, mouseProps.y);
    int cellX = coords[0];
    int cellY = coords[1];
    if (cellX >= 0 && cellX < _grid.size)
    {
        if (cellY >= 0 && cellY < _grid.size)
        {
            // toggle cell if if window coords are within grid bounds
            mouseProps.cellsToggled.push_back({cellX, cellY});
            return _grid.toggleCell(cellX, cellY);
        };
    };
}

Coords Game::getCellCoordinates(int x, int y)
{
    int cellSize = _grid.cellSize;
    double cellY, cellX;
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

        // calculate new cell width after rotation (hypotenuse of triangle after 45 deg rotation)
        double l = pow(cellSize / 2, 2); // leg length
        double newCellWidth = sqrt(l * 2);

        // anti-clockwise rotation of 315deg to get a clock-wise rotation of 45deg
        point p(a, b);
        point n = p * std::polar(1.0, 1.75 * PI);

        // set new margin after rotation
        double margin = (_config.width - (newCellWidth * _grid.size)) / 2;

        cellX = ((n.real() + xCenter) - margin) / newCellWidth;
        cellY = ((yCenter - n.imag()) - margin) / newCellWidth;
    }
    else
    {
        cellX = (x - _grid.marginX) / cellSize;
        cellY = (y - _grid.marginY) / cellSize;
    }

    Coords coords = {int(floor(cellX)), int(floor(cellY))};
    return coords;
}

void Game::clean()
{
    _renderer.tearDown();
    SDL_DestroyWindow(window);
    SDL_Quit();
    TTF_Quit();
    std::cout
        << "game quit" << std::endl;
}

void Game::handleTicks()
{
    _ticks--;
    if (_ticks <= 0)
    {
        if (!isPaused && _grid.population > 0)
        {
            _grid.cycleGeneration();
        }
        _ticks = _ticksPerGeneration;
    }
}

// return current tick as percentage of total ticks
int Game::getCurrentTickAsPercentage()
{
    return (static_cast<float>(_ticks) / static_cast<float>(_ticksPerGeneration)) * 100;
}

void Game::init(Config config)
{
    int flags = 0;
    if (config.fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    initFont();

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

void Game::initFont()
{
    if (TTF_Init() < 0)
    {
        printf("Couldn't initialize SDL TTF: %s\n", SDL_GetError());
        exit(1);
    }

    auto f = TTF_OpenFont("Geneva.ttf", _config.fontSize);
    if (!font)
    {
        printf("Couldn't initialize font: %s\n", SDL_GetError());
    }
    font = f;
}
