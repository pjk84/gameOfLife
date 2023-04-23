
#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <tuple>
#include <vector>
#include <array>

namespace GameOfLife
{
    typedef std::array<std::vector<SDL_Vertex>, 2> SquareIso;
    struct textProps
    {
        int x;
        int y;
        const char *body;
        SDL_Color color;
        TTF_Font *font;
    };
    class Grid;
    class Renderer
    {
    private:
        const int _width;
        SDL_Rect r;
        SDL_Renderer *_renderer;
        SquareIso squareIsometric{};

    public:
        bool animateCursor = true;
        void renderText(textProps text);
        void renderBackground();
        void renderCursor();
        void renderClear();
        void tearDown();
        void initialize(SDL_Window *window);
        void renderGridFlat(Grid &grid);
        void renderGridIsometric(Grid &grid, std::array<int, 2> cursor, int ticks);
        SDL_Color getCellColor(bool isAlive, int index, int ticks, int effect);
        void renderCell(int x, int y, int cellSize);
        void copyCell(int x, int y);
        Renderer(int width) : _width{width} {}

        void initSquareIsometric(Grid &grid);
    };
}

#endif