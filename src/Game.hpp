

#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include <list>
#include <SDL2/SDL.h>
#include "Grid.hpp"
#include "Renderer.hpp"

namespace GameOfLife
{
    struct Settings
    {
        const char *title;
        int xPos;
        int yPos;
        int width;
        int height;
        int gridSize;
        bool fullscreen;
    };

    class Game
    {
    public:
        bool isRunning = false;
        Game(Settings settings);
        ~Game();
        void printGrid();
        void init(Settings);
        void handleEvents();
        // void update();
        void render();
        void clean();
        void handleGeneration();
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
        Grid _grid;
        Renderer _renderer;
        int _ticksPerGeneration = 20;
        int _ticks;
        const char *_title;
        int _xPos;
        int _yPos;
        int _height;
        int _width;
        bool _fullscreen;
        // std::vector<Shape> _shapes;
        SDL_Window *window;
        void renderBackground();
        void renderGrid();
    };
}

#endif