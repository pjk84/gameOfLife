#define _XOPEN_SOURCE_EXTENDED 1
#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <cctype>
#include "Game.hpp"
#include <sstream>
#include <chrono>
#include <thread>

using namespace GameOfLife;

Game::Game(int gridSize)
{
    _gridSize = gridSize;
    seed(gridSize);
    run();
}

void Game::run()
{
    while (_isPlaying)
    {
        printGrid();
        generate();

        _gridArrayIndex = 1 - _gridArrayIndex;
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

void Game::generate()
{
    _numberLiving = 0;
    _generation++;
    auto &thisGenGrid = _gridArray[_gridArrayIndex];
    auto &nextGenGrid = _gridArray[1 - _gridArrayIndex];
    for (int i = 0; i < _gridSize; ++i)
    {
        bool lookUp = false;
        if (i > 0)
        {
            lookUp = true;
        }
        bool lookDown = false;
        if (i < _gridSize - 1)
        {
            lookDown = true;
        }
        for (int n = 0; n < _gridSize; n++)
        {
            int sum = 0;
            if (n < _gridSize - 1)
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
                if (lookUp && thisGenGrid.at(i)[n - 1] == 1)
                {
                    sum++;
                }
                // 7h neighbour
                if (lookDown && thisGenGrid.at(i)[n - 1] == 1)
                {
                    sum++;
                }
            }
            if (i < _gridSize - 1)
            {
                // 6h neighbour
                if (lookDown && thisGenGrid.at(i)[n] == 1)
                {
                    sum++;
                }
            }
            if (i > 0)
            {
                // 12h neighbour
                if (lookUp && thisGenGrid.at(i)[n] == 1)
                {
                    sum++;
                }
            }
            if (thisGenGrid.at(i)[n] == 1)
            {
                // living cell
                _numberLiving++;
                if (sum != 2 && sum != 3)
                {
                    // kill cell
                    nextGenGrid.at(i)[n] = 0;
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
}

void Game::seed(int gridSize)
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
    _isPlaying = true;
}

void Game::printGrid()
{
    std::cout << "generation:" << std::to_string(_generation) << " number alive:" << std::to_string(_numberLiving) << std::endl;
    for (int i = 0; i < _gridSize; ++i)
    {
        auto v = _gridArray[_gridArrayIndex].at(i);
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
    return;
}

int main()
{
    GameOfLife::Game g(30);
    return 0;
}
