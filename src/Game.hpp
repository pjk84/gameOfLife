

#ifndef GAME_H
#define GAME_H
#include <string>
#include <vector>
#include <list>
#include <SDL2/SDL.h>
// #include <SDL2_ttf/SDL_ttf.h>
#include <complex>
#include "Grid.hpp"
#include "Renderer.hpp"
#include <tuple>

namespace GameOfLife
{
    typedef std::complex<double> point;
    typedef std::array<int, 2> Coords;
    struct Config
    {
        const char *title;
        int xPos;
        int yPos;
        int height;
        int width;
        int gridSize;
        bool fullscreen;
        int fontSize = 24;
    };

    struct MouseProps
    {
        int x;
        int y;
        bool isDown = false;
        std::vector<Coords> cellsToggled{};
    };

    class Game
    {
    public:
        bool isRunning = false;
        bool isPaused = false;
        bool isoMetric = false;
        TTF_Font *font;
        MouseProps mouseProps;
        Game(Config settings);
        ~Game();
        void printGrid();
        void init(Config);
        void handleEvents();
        void render(int ticks);
        void renderText();
        void clean();
        void handleTicks();
        int getCurrentTickAsPercentage();
        Coords getCellCoordinates(int x, int y);
        void handleMouseButtonDown(uint8_t buttonIndex);
        void handleMouseButtonUp();
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
        void initFont();
        void toggleCell();
    };
}

#endif