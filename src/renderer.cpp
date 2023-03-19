#include "Renderer.hpp"
#include <math.h>
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

using namespace GameOfLife;

SDL_Rect r;

Renderer::Renderer(int width)
    : _width{width}
{
}

void Renderer::renderGrid(std::vector<std::vector<int>> &grid)
{
    int gridSize = grid[0].size();
    int cellSize = ceil(_width / gridSize);
    int margin = (_width - cellSize * gridSize) / 2;
    int lineWidth = ceil(cellSize / 10000);
    if (lineWidth < 1)
    {
        lineWidth = 1;
    }

    r.h = cellSize - lineWidth;
    r.w = cellSize - lineWidth;
    int yPos = margin;
    // std::cout << "generation:" << std::to_string(_generation) << " number alive:" << std::to_string(_numberLiving) << std::endl;
    for (int i = 0; i < gridSize; ++i)
    {
        int xPos = margin;
        if (i > 0)
        {
            // shift one cell down
            yPos += cellSize;
        }
        auto &v = grid[i];
        for (int n = 0; n < gridSize; ++n)
        {
            if (n > 0)
            {
                // shift one cell to right
                xPos += cellSize;
            }
            int color = v[n] == 1 ? 255 : 0;
            r.x = xPos;
            r.y = yPos;
            SDL_SetRenderDrawColor(_renderer, color, color, color, color);
            SDL_RenderFillRect(_renderer, &r);
            // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
            // SDL_RenderDrawPoint(renderer, 10, 10);
        }
    }
    return;
}

void Renderer::renderBackground()
{
    // simple white canvas for now
    SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
    SDL_RenderClear(_renderer);
}

void Renderer::renderClear()
{
    SDL_RenderPresent(_renderer);
    SDL_RenderClear(_renderer);
}

void Renderer::tearDown()
{
    SDL_DestroyRenderer(_renderer);
}

void Renderer::initialize(SDL_Window *window)
{
    _renderer = SDL_CreateRenderer(window, -1, 0);
    if (_renderer)
    {
        std::cout << "renderer initialized" << std::endl;
    }
}