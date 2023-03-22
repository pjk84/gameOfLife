
#ifndef GRID_H
#define GRID_H
#include <vector>

namespace GameOfLife
{
    class Grid
    {

    private:
        int _generation = 0;
        int _numberLiving = 0;
        void seed(int rows, int cols);
        void run();

    public:
        Grid(int rows, int cols, int width, int height);
        void cycleGeneration();
        void printGrid();
        void toggleCell(int x, int y);
        const int rows;
        const int cols;
        int cellSize;
        int marginH;
        int marginV;
        int gridArrayIndex = 0;
        std::vector<std::vector<int>> gridArray[2];
    };

}

#endif