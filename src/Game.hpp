
#include <string>
#include <vector>
#include <list>
#include <SDL2/SDL.h>

namespace GameOfLife
{

    // class Game
    // {

    // private:
    //     int _generation = 0;
    //     int _numberLiving = 0;
    //     bool _isPlaying;
    //     int _gridSize;
    //     int _gridArrayIndex = 0;
    //     std::vector<std::vector<int>> _gridArray[2];
    //     void seed();
    //     void run();
    //     void generate();

    // public:
    //     Game(int gridSize);
    //     std::string test();
    //     void seed(int gridSize);
    //     void printGrid();
    // };

    class Game
    {
    public:
        bool _isRunning = false;
        Game(const char *title, int xPos, int yPos, int height, int width, bool fullscreen);
        ~Game();
        // void init();
        void handleEvents();
        // void update();
        void render();
        void clean();
        void renderBackground();
        // void renderShape(const Shape &s);
        // void makeShape();
        // void checkCollision(Shape &s);
        // void handleMouseInput(uint8_t buttonIndex);
        // void grabShape(int x, int y);
        // void releaseAllShapes();
        // void dragShape();
        // void handleMouseMotion(Sint32 x, Sint32 y);
        std::string concat(std::vector<std::vector<int>>);
        std::list<int> activeShapes;
        std::list<std::vector<int>> coords;

    private:
        const char *_title;
        int _xPos;
        int _yPos;
        int _height;
        int _width;
        bool _fullscreen;
        // std::vector<Shape> _shapes;
        int _count = 0;
        bool _isrunning = false;
        SDL_Window *window;
        SDL_Renderer *renderer;
    };
}