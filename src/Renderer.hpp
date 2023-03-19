
#ifndef RENDERER_H
#define RENDERER_H
#include <SDL2/SDL.h>
#include <vector>

namespace GameOfLife
{
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
        void renderGrid(std::vector<std::vector<int>> &grid);
        Renderer(int windowWidth);
    };
}

#endif