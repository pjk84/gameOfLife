#include "Renderer.hpp"
#include "Grid.hpp"
#include <math.h>
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <algorithm>

using namespace GameOfLife;

void Renderer::renderGrid(Grid &grid)
{
    // the grid to render
    auto g = grid.gridArray[grid.gridArrayIndex];
    int cellSize = grid.cellSize;
    int lineWidth = std::max(int(ceil(grid.rows / 10000)), 1);

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
        r.y = yPos;
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
            if (v[n] == 1)
            {
                if (cellSize < 20)
                {
                    // render basic rect
                    SDL_SetRenderDrawColor(_renderer, 100, 100, 100, SDL_ALPHA_OPAQUE);
                    SDL_RenderFillRect(_renderer, &r);
                }
                else
                {
                    // render gradient cell
                    renderCell(xPos, yPos, cellSize - lineWidth);
                }
            }
            else
            {
                SDL_SetRenderDrawColor(_renderer, 222, 222, 222, SDL_ALPHA_OPAQUE);
                SDL_RenderFillRect(_renderer, &r);
            }
        }
    }
    return;
}

void Renderer::renderCell(int x, int y, int cellSize)
{
    SDL_Rect rr = r;
    int c = 200;
    rr.h = 1;
    for (int i = 0; i < cellSize; i++)
    {
        c -= (i > cellSize / 2 ? -1 : 1);
        SDL_SetRenderDrawColor(_renderer, 0, c, 0, 10);
        SDL_RenderFillRect(_renderer, &rr);
        rr.y++;
    }
}

void Renderer::renderBackground()
{
    // simple white canvas for now
    SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 0);
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