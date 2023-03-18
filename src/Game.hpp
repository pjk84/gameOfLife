
#include <string>
#include <vector>

namespace GameOfLife
{

    class Game
    {

    private:
        int _generation = 0;
        int _numberLiving = 0;
        bool _isPlaying;
        int _gridSize;
        int _gridArrayIndex = 0;
        std::vector<std::vector<int>> _gridArray[2];
        void seed();
        void run();
        void generate();

    public:
        Game(int gridSize);
        std::string test();
        void seed(int gridSize);
        void printGrid();
    };
}