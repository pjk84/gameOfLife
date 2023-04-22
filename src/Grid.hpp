
#ifndef GRID_H
#define GRID_H
#include <vector>

namespace GameOfLife
{
    class Grid
    {

    private:
        void seed(int gridSize);
        void run();

    public:
        Grid(int size, int width, int height);
        int generation = 0;
        int population = 0;
        void cycleGeneration();
        void printGrid();
        void toggleCell(int x, int y);
        int cellSize;
        int size;
        int marginY;
        int marginX;
        int gridArrayIndex = 0;
        std::vector<std::vector<int>> gridArray[2];
    };

}

#endif