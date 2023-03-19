
#include <vector>

namespace GameOfLife
{
    class Grid
    {

    private:
        int _generation = 0;
        int _numberLiving = 0;
        void seed(int gridSize);
        void run();

    public:
        Grid(int gridSize);
        void cycleGeneration();
        void printGrid();
        int gridSize;
        int gridArrayIndex = 0;
        std::vector<std::vector<int>> _gridArray[2];
    };

}