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

using namespace GameOfLife;

Grid::Grid(int size)
{
    gridSize = size;
    seed(gridSize);
}

void Grid::cycleGeneration()
{
    _numberLiving = 0;
    _generation++;
    auto &thisGenGrid = _gridArray[gridArrayIndex];
    auto &nextGenGrid = _gridArray[1 - gridArrayIndex];

    for (int i = 0; i < gridSize; ++i)
    {
        bool lookUp = false;
        if (i > 0)
        {
            lookUp = true;
        }
        bool lookDown = false;
        if (i < gridSize - 1)
        {
            lookDown = true;
        }
        for (int n = 0; n < gridSize; n++)
        {
            int sum = 0;
            if (n < gridSize - 1)
            {
                // 3h neighbour
                if (thisGenGrid.at(i)[n + 1] == 1)
                {
                    sum++;
                }
                // 1h neighbour
                if (lookUp && thisGenGrid.at(i - 1)[n + 1] == 1)
                {
                    sum++;
                }
                // 5h neighbour
                if (lookDown && thisGenGrid.at(i + 1)[n + 1] == 1)
                {
                    sum++;
                }
            }
            if (n > 0)
            {
                // 9h neighbour
                if (thisGenGrid.at(i)[n - 1] == 1)
                {
                    sum++;
                }
                // 11h neighbour
                if (lookUp && thisGenGrid.at(i - 1)[n - 1] == 1)
                {
                    sum++;
                }
                // 7h neighbour
                if (lookDown && thisGenGrid.at(i + 1)[n - 1] == 1)
                {
                    sum++;
                }
            }
            if (i < gridSize - 1)
            {
                // 6h neighbour
                if (lookDown && thisGenGrid.at(i + 1)[n] == 1)
                {
                    sum++;
                }
            }
            if (i > 0)
            {
                // 12h neighbour
                if (lookUp && thisGenGrid.at(i - 1)[n] == 1)
                {
                    sum++;
                }
            }
            if (i == 4 && n == 11)
            {
                std::cout << sum << std::endl;
            }
            if (thisGenGrid.at(i)[n] == 1)
            {
                // living cell
                _numberLiving++;
                if (sum != 2 && sum != 3)
                {
                    // kill cell
                    nextGenGrid.at(i)[n] = 3;
                }
            }
            else
            {

                // empty cell
                if (sum == 3)
                {
                    // spawn new cell
                    nextGenGrid.at(i)[n] = 1;
                }
            }
        }
    }
    printGrid();
    gridArrayIndex = 1 - gridArrayIndex;
}

void Grid::seed(int gridSize)
{
    // create row with empty cells
    int b;
    std::vector<std::vector<int>> grid;
    for (int i = 0; i < gridSize; ++i)
    {
        std::vector<int> v;
        for (int n = 0; n < gridSize; n++)
        {
            int r = rand() % 10;
            if (r == 1)
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
    _gridArray[0] = grid;
    _gridArray[1] = gridCopy;
}

void Grid::printGrid()
{
    std::cout << "generation:" << std::to_string(_generation) << " number alive:" << std::to_string(_numberLiving) << std::endl;
    for (int i = 0; i < gridSize; ++i)
    {
        auto v = _gridArray[gridArrayIndex].at(i);
        std::stringstream s;
        for (auto it = v.begin(); it != v.end(); it++)
        {
            if (it != v.begin())
            {
                s << " ";
            }
            s << *it;
        }
        std::cout << s.str() << " " << std::to_string(i) << "\n";
    }
    std::string x = "";
    for (int i = 0; i < gridSize; ++i)
    {
        x += std::to_string(i) + " ";
    }
    std::cout << x << std::endl;
    return;
}