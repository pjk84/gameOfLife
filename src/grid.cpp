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

Grid::Grid(int rows, int cols, int width, int height)
    : rows{rows}, cols{cols}
{
    cellSize = height < width ? height / rows : width / cols;

    // round to nearest multiple of 4.
    cellSize = round((cellSize / 2) / 2) * 4;
    marginY = (height - (cellSize * rows)) / 2;
    marginX = (width - (cellSize * cols)) / 2;

    seed(rows, cols);
}

void Grid::cycleGeneration()
{
    _numberLiving = 0;
    _generation++;
    auto &thisGenGrid = gridArray[gridArrayIndex];
    auto &nextGenGrid = gridArray[1 - gridArrayIndex];

    for (int i = 0; i < rows; ++i)
    {
        bool lookUp = false;
        if (i > 0)
        {
            lookUp = true;
        }
        bool lookDown = false;
        if (i < rows - 1)
        {
            lookDown = true;
        }
        for (int n = 0; n < cols; n++)
        {
            int sum = 0;
            if (n < cols - 1)
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
            if (i < rows - 1)
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
                _numberLiving++;
                if (sum != 2 && sum != 3)
                {
                    // kill cell
                    nextGenGrid[i][n] = 0;
                }
                else
                {
                    // survives
                    nextGenGrid[i][n] = 1;
                }
            }
            else
            {

                // empty cell
                if (sum == 3)
                {
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
    auto &v = gridArray[gridArrayIndex];
    v[y][x] = v[y][x] == 0 ? 1 : 0;
}

void Grid::seed(int rows, int cols)
{
    // create row with empty cells
    int b;
    std::vector<std::vector<int>> grid;
    for (int i = 0; i < rows; ++i)
    {
        std::vector<int> v;
        for (int n = 0; n < cols; n++)
        {
            int r = rand() % 10;
            if (r < 3)
            {
                b = 1;
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
