

#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include <list>
#include <SDL2/SDL.h>
#include "Grid.hpp"
#include "Renderer.hpp"
#include <tuple>

namespace GameOfLife
{
    struct Config
    {
        const char *title;
        int xPos;
        int yPos;
        int height;
        int width;
        int gridSize;
        bool fullscreen;
    };

    class Game
    {
    public:
        bool isRunning = false;
        bool isPaused = false;
        bool isoMetric = false;
        int mouseX;
        int mouseY;
        Game(Config settings);
        ~Game();
        void printGrid();
        void init(Config);
        void handleEvents();
        void render(int ticks);
        void clean();
        void handleTicks();
        int getCurrentTickAsPercentage();
        std::tuple<int, int> getCellCoordinates(int x, int y);
        void handleMouseButtonDown(uint8_t buttonIndex);
        void handleMouseMotionEvent(SDL_MouseMotionEvent event);
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
        void renderBackground();
        void renderGrid();
        void toggleCell(int x, int y);
    };
}

#endif