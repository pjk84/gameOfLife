

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
    struct Config
    {
        const char *title;
        int xPos;
        int yPos;
        int width;
        int height;
        int rows;
        int cols;
        bool fullscreen;
    };

    class Game
    {
    public:
        bool isRunning = false;
        bool isPaused = false;
        Game(Config settings);
        ~Game();
        void printGrid();
        void init(Config);
        void handleEvents();
        void render();
        void clean();
        void handleGeneration();
        void handleMouseInput(uint8_t buttonIndex);
        std::string concat(std::vector<std::vector<int>>);
        std::list<int> activeShapes;
        std::list<std::vector<int>> coords;

    private:
        Config _config;
        SDL_Window *window;
        Grid _grid;
        Renderer _renderer;
        int _ticksPerGeneration = 25;
        int _ticks;
        const char *_title;
        // std::vector<Shape> _shapes
        void renderBackground();
        void renderGrid();
        void toggleCell(int x, int y);
    };
}

#endif