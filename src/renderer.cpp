#include "Renderer.hpp"
#include "Grid.hpp"
#include <math.h>
#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <algorithm>
#include <tuple>

using namespace GameOfLife;

void Renderer::renderGridFlat(Grid &grid)
{
    // the grid to render
    auto g = grid.gridArray[grid.gridArrayIndex];
    int cellSize = grid.cellSize;
    int lineWidth = std::max(int(ceil(grid.size / 10000)), 1);

    r.h = cellSize - lineWidth;
    r.w = cellSize - lineWidth;
    int yPos = grid.marginY;
    for (int i = 0; i < grid.size; ++i)
    {
        int xPos = grid.marginX;
        if (i > 0)
        {
            // shift one cell down
            yPos += cellSize;
        }
        r.y = yPos;
        auto &v = g[i];
        for (int n = 0; n < grid.size; ++n)
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

void Renderer::renderGridIsometric(Grid &grid, std::tuple<int, int> cursor)
{
    // the grid to render
    auto g = grid.gridArray[grid.gridArrayIndex];
    int lineWidth = grid.size * grid.size >= 400 ? 0 : 2;
    float16_t cellSize = grid.cellSize - (lineWidth * 2);
    float16_t yPos = grid.marginY + lineWidth + ((grid.cellSize * grid.size) / 2);
    float16_t xPos = grid.marginX + lineWidth;

    SDL_FPoint textPos = SDL_FPoint{0};
    SDL_FPoint pTop = SDL_FPoint{xPos + cellSize / 2, yPos - (cellSize / 4)};
    SDL_FPoint pBotton = SDL_FPoint{xPos + cellSize / 2, yPos + (cellSize / 4)};
    SDL_FPoint pLeft = SDL_FPoint{xPos, yPos};
    SDL_FPoint pRight = SDL_FPoint{xPos + cellSize, yPos};
    SDL_Color colorOff = SDL_Color{222, 222, 222, SDL_ALPHA_OPAQUE};
    SDL_Color colorOn = SDL_Color{222, 222, 222, SDL_ALPHA_OPAQUE};
    std::vector<SDL_Vertex> vertsLeft =
        {
            {
                pTop,
                colorOff,
                SDL_FPoint{0},
            },
            {
                pBotton,
                colorOff,
                SDL_FPoint{0},
            },
            {
                pLeft,
                colorOff,
                SDL_FPoint{0},
            },

        };
    std::vector<SDL_Vertex> vertsRight =
        {
            {
                pTop,
                colorOff,
                SDL_FPoint{0},
            },
            {
                pBotton,
                colorOff,
                SDL_FPoint{0},
            },
            {
                pRight,
                colorOff,
                SDL_FPoint{0},
            },
        };
    int shiftWidth = ((grid.cellSize * grid.size) / 2);
    int shiftHeight = shiftWidth / 2;

    int z = 0;
    for (int i = 0; i < grid.size; ++i)
    {
        z++;
        if (i > 0)
        {
            for (auto &v : vertsLeft)
            {
                v.position.x -= shiftWidth - grid.cellSize;
                v.position.y += shiftHeight;
            }
            for (auto &v : vertsRight)
            {
                v.position.x -= shiftWidth - grid.cellSize;
                v.position.y += shiftHeight;
            }
        }

        for (int n = 0; n < grid.size; ++n)
        {

            bool showCursor = std::get<0>(cursor) == n && std::get<1>(cursor) == i;
            auto &v = g[i];
            int value = v[n];
            // SDL_Color c = value == 1 ? SDL_Color{0, 200, 0, SDL_ALPHA_OPAQUE} : n % 2 == z ? SDL_Color{255, 255, 255, SDL_ALPHA_OPAQUE}
            //                                                                                : SDL_Color{222, 222, 222, SDL_ALPHA_OPAQUE};

            // if (showCursor)
            // {
            //     c = SDL_Color{0, 100, 50, SDL_ALPHA_OPAQUE};
            // }

            auto c = getCellColor(value == 1, showCursor, z);

            float16_t xShift = n > 0 ? ((cellSize / 2) + lineWidth) : 0;
            float16_t yShift = n > 0 ? ((cellSize / 4) + lineWidth / 2) : 0;
            for (auto &v : vertsLeft)
            {
                v.color = c;
                v.position.x += xShift;
                v.position.y -= yShift;
            }
            for (auto &v : vertsRight)
            {
                v.color = c;
                v.position.x += xShift;
                v.position.y -= yShift;
            }

            {
                SDL_RenderGeometry(_renderer, NULL, vertsLeft.data(), vertsLeft.size(), nullptr, 0);
                SDL_RenderGeometry(_renderer, NULL, vertsRight.data(), vertsRight.size(), nullptr, 0);
            }
            z++;
        }
    }
    return;
}

SDL_Color Renderer::getCellColor(bool isAlive, bool hasCursor, int index)
{
    SDL_Color c = isAlive ? SDL_Color{0, 100, 50, SDL_ALPHA_OPAQUE} : index % 2 == 0 ? SDL_Color{255, 255, 255, SDL_ALPHA_OPAQUE}
                                                                                     : SDL_Color{222, 222, 222, SDL_ALPHA_OPAQUE};

    if (hasCursor)
    {
        c = SDL_Color{0, 200, 0, SDL_ALPHA_OPAQUE};
    }
    return c;
}

void Renderer::renderCell(int x, int y, int cellSize)
{
    SDL_Rect rr = r;
    int c = 200;
    rr.h = 1;
    for (int i = 0; i < cellSize; i++)
    {
        // c -= (i > cellSize / 2 ? -1 : 1);
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

void Renderer::renderCursor()
{
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