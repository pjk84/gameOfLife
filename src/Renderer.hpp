
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
        const int _width;
        SDL_Rect r;
        SDL_Renderer *_renderer;

    public:
        void renderBackground();
        void renderCursor();
        void renderClear();
        void tearDown();
        void initialize(SDL_Window *window);
        void renderGridFlat(Grid &grid);
        void renderGridIsometric(Grid &grid, std::tuple<int, int> cursor);
        SDL_Color getCellColor(bool isAlive, bool hasCursor, int);
        void renderCell(int x, int y, int cellSize);
        void copyCell(int x, int y);
        Renderer(int width) : _width{width} {}
    };
}

#endif