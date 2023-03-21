
#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include <vector>

namespace GameOfLife
{
    class Grid;
    class Renderer
    {
    private:
        int _width;
        SDL_Renderer *_renderer;

    public:
        void renderBackground();
        void renderClear();
        void tearDown();
        void initialize(SDL_Window *window);
        void renderGrid(Grid &grid);
        void renderCell(int x, int y, int cellSize);
        Renderer(int windowWidth);
    };
}

#endif