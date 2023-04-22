#define _XOPEN_SOURCE_EXTENDED 1
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cctype>
#include "Grid.hpp"
#include <sstream>
#include <chrono>
#include <thread>
#include <math.h>

using namespace GameOfLife;

Grid::Grid(int size, int width, int height)
    : size(size)
{
    cellSize = height < width ? height / size : width / size;
    // round to nearest multiple of 4.
    cellSize = round((cellSize / 2) / 2) * 4;
    marginY = (height - (cellSize * size)) / 2;
    marginX = (width - (cellSize * size)) / 2;

    seed(size);
}

void Grid::cycleGeneration()
{
    population = 0;
    generation++;
    auto &thisGenGrid = gridArray[gridArrayIndex];
    auto &nextGenGrid = gridArray[1 - gridArrayIndex];

    for (int i = 0; i < size; ++i)
    {
        bool lookUp = false;
        if (i > 0)
        {
            lookUp = true;
        }
        bool lookDown = false;
        if (i < size - 1)
        {
            lookDown = true;
        }
        for (int n = 0; n < size; n++)
        {
            int sum = 0;
            if (n < size - 1)
            {
                // 3h neighbour
                if (thisGenGrid[i][n + 1] == 1)
                {
                    sum++;
                }
                // 1h neighbour
                if (lookUp && thisGenGrid[i - 1][n + 1] == 1)
                {
                    sum++;
                }
                // 5h neighbour
                if (lookDown && thisGenGrid[i + 1][n + 1] == 1)
                {
                    sum++;
                }
            }
            if (n > 0)
            {
                // 9h neighbour
                if (thisGenGrid[i][n - 1] == 1)
                {
                    sum++;
                }
                // 11h neighbour
                if (lookUp && thisGenGrid[i - 1][n - 1] == 1)
                {
                    sum++;
                }
                // 7h neighbour
                if (lookDown && thisGenGrid[i + 1][n - 1] == 1)
                {
                    sum++;
                }
            }
            if (i < size - 1)
            {
                // 6h neighbour
                if (lookDown && thisGenGrid[i + 1][n] == 1)
                {
                    sum++;
                }
            }
            if (i > 0)
            {
                // 12h neighbour
                if (lookUp && thisGenGrid[i - 1][n] == 1)
                {
                    sum++;
                }
            }

            if (thisGenGrid[i][n] == 1)
            {

                // living cell
                if (sum != 2 && sum != 3)
                {
                    // kill cell
                    nextGenGrid[i][n] = 0;
                }
                else
                {
                    population++;
                    // survives
                    nextGenGrid[i][n] = 1;
                }
            }
            else
            {

                // empty cell
                if (sum == 3)
                {
                    population++;
                    // spawn new cell
                    nextGenGrid[i][n] = 1;
                }
                else
                {
                    // stays empty
                    nextGenGrid[i][n] = 0;
                }
            }
        }
    }
    gridArrayIndex = 1 - gridArrayIndex;
}

void Grid::toggleCell(int x, int y)
{
    auto &grid = gridArray[gridArrayIndex];
    auto v = grid[y][x];
    grid[y][x] = 1 - v;
    if (v == 1)
    {
        population--;
    }
    else
    {
        population++;
    }
}

void Grid::seed(int size)
{
    // create row with empty cells
    int b;
    std::vector<std::vector<int>> grid;
    for (int i = 0; i < size; ++i)
    {
        std::vector<int> v;
        for (int n = 0; n < size; n++)
        {
            int r = rand() % 10;
            if (r < 3)
            {
                b = 1;
                population += 1;
            }
            else
            {
                b = 0;
            }
            v.push_back(b);
        }
        // add row to grid
        grid.push_back(v);
    }
    auto gridCopy = grid;
    gridArray[0] = grid;
    gridArray[1] = gridCopy;
}
