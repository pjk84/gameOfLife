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
      _grid{Grid(config.gridSize)},
      _renderer{Renderer(ceil(config.width))}
{
    _ticks = _ticksPerGeneration;
    init(config);
};

Game::~Game()
{
    std::cout << "destroyed" << std::endl;
}

// void Game::renderGrid()
// {
//     int cellSize = ceil(_width / _grid.gridSize);
//     int lineWidth = ceil(cellSize / 10000);
//     if (lineWidth < 1)
//     {
//         lineWidth = 1;
//     }
//     int margin = (_width - cellSize * _grid.gridSize) / 2;
//     r.h = cellSize - lineWidth;
//     r.w = cellSize - lineWidth;
//     int yPos = margin;
//     // std::cout << "generation:" << std::to_string(_generation) << " number alive:" << std::to_string(_numberLiving) << std::endl;
//     for (int i = 0; i < _grid.gridSize; ++i)
//     {
//         int xPos = margin;
//         if (i > 0)
//         {
//             // shift one cell down
//             yPos += cellSize;
//         }
//         auto &v = _grid._gridArray[_grid.gridArrayIndex].at(i);
//         for (int n = 0; n < _grid.gridSize; ++n)
//         {
//             if (n > 0)
//             {
//                 // shift one cell to right
//                 xPos += cellSize;
//             }
//             int color = v.at(n) == 1 ? 255 : 0;
//             r.x = xPos;
//             r.y = yPos;
//             SDL_SetRenderDrawColor(renderer, color, color, color, color);
//             SDL_RenderFillRect(renderer, &r);
//             // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
//             // SDL_RenderDrawPoint(renderer, 10, 10);
//         }
//     }
//     return;
// }

// void Game::renderCell(const Shape& s){
//     r.x = s.xPos;
//     r.y = s.yPos;
//     r.h = s.height;
//     r.w = s.width;
//     SDL_SetRenderDrawColor(renderer, s.color[0], s.color[1], s.color[2], s.color[3]);
//     SDL_RenderFillRect(renderer, &r);
//     // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
//     // SDL_RenderDrawPoint(renderer, 10, 10);
// }

void Game::render()
{
    auto &grid = _grid.gridArray[_grid.gridArrayIndex];
    _renderer.renderBackground();
    _renderer.renderGrid(grid);
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
        int cellX = floor(x / _grid.gridSize);
        int cellY = floor(y / _grid.gridSize);
        _grid.toggleCell(cellX, cellY);
    }
}

// void Game::releaseAllShapes(){
//     for(auto & shape: _shapes){
//         shape.setGrabbed(false);
//     }
//     activeShapes.clear();
// }

// void Game::update(){

//     if(_shapes.size() == 0){
//         makeShape();
//     }
//     for(auto &shape: _shapes){
//         if(shape.moving){
//             shape.setCoords(shape.xPos+(shape.xDelta * shape.velocity), shape.yPos+(shape.yDelta * shape.velocity));
//             checkCollision(shape);
//         }
//     }
// }

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