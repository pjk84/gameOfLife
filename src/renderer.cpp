#include "Renderer.hpp"
#include "Grid.hpp"
#include <math.h>
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>

using namespace GameOfLife;

SDL_Rect r;

Renderer::Renderer(int width)
    : _width{width}
{
}

void Renderer::renderGrid(Grid &grid)
{
    // the grid to render
    auto g = grid.gridArray[grid.gridArrayIndex];
    int cellSize = grid.cellSize;
    int lineWidth = ceil(grid.rows / 10000);
    if (lineWidth < 1)
    {
        lineWidth = 1;
    }

    r.h = cellSize - lineWidth;
    r.w = cellSize - lineWidth;
    int yPos = grid.marginH;
    // std::cout << "generation:" << std::to_string(_generation) << " number alive:" << std::to_string(_numberLiving) << std::endl;
    for (int i = 0; i < grid.rows; ++i)
    {
        int xPos = grid.marginV;
        if (i > 0)
        {
            // shift one cell down
            yPos += cellSize;
        }
        auto &v = g[i];
        for (int n = 0; n < grid.cols; ++n)
        {
            if (n > 0)
            {
                // shift one cell to right
                xPos += cellSize;
            }
            int color = v[n] == 1 ? 255 : 0;
            r.x = xPos;
            r.y = yPos;
            if (v[n] == 1)
            {
                renderCell(xPos, yPos, cellSize);
            }
            else
            {
                SDL_SetRenderDrawColor(_renderer, 255, 255, 100, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(_renderer, &r);
            }
            // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
            // SDL_RenderDrawPoint(renderer, 10, 10);
        }
    }
    return;
}

void Renderer::renderCell(int x, int y, int cellSize)
{
    int c = 255;
    int center = cellSize / 2;
    int depth = ceil(cellSize / 4) / 2;
    for (int i = 0; i < cellSize; i++)
    {
        y++;
        for (int n = 0; n < cellSize; n++)
        {
            x++;
            int d = center - std::max(abs(center - x), abs(center - y));

            int cc = c - (ceil(d / depth) * 25);

            SDL_SetRenderDrawColor(_renderer, cc, 0, 0, SDL_ALPHA_OPAQUE);
            SDL_RenderDrawPoint(_renderer, x, y);
        }
        x -= cellSize;
    }
}

void Renderer::renderBackground()
{
    // simple white canvas for now
    SDL_SetRenderDrawColor(_renderer, 100, 100, 1090, 100);
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