#define _XOPEN_SOURCE_EXTENDED 1
#include "Game.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>

using namespace GameOfLife;

SDL_Rect r;

Game::Game(const char *title, int xPos, int yPos, int height, int width, bool fullscreen)
    : _title{title}, _xPos(xPos), _yPos{yPos}, _height{height}, _width{width}, _fullscreen{fullscreen}
{
    int flags = 0;
    if (fullscreen)
    {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        std::cout << "sdl initialized..." << std::endl;
        window = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
        if (window)
        {
            std::cout << "window created" << std::endl;
        }
        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer)
        {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            std::cout << "renderer initialized" << std::endl;
        }
        _isRunning = true;
    }
};

Game::~Game()
{
}

void Game::renderBackground()
{
    // simple white canvas for now
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
}

// void Game::renderShape(const Shape& s){
//     r.x = s.xPos;
//     r.y = s.yPos;
//     r.h = s.height;
//     r.w = s.width;
//     SDL_SetRenderDrawColor(renderer, s.color[0], s.color[1], s.color[2], s.color[3]);
//     SDL_RenderFillRect(renderer, &r);
//     // SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
//     // SDL_RenderDrawPoint(renderer, 10, 10);
// }

void Game::render()
{
    renderBackground();

    SDL_RenderPresent(renderer);
}

void Game::handleEvents()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        _isRunning = false;
        break;
    // case SDL_MOUSEBUTTONDOWN:
    //     handleMouseInput(event.button.button);
    //     break;
    // case SDL_MOUSEBUTTONUP:
    //     releaseAllShapes();
    //     break;
    // case SDL_MOUSEMOTION:
    //     handleMouseMotion(event.motion.x, event.motion.y);
    default:
        break;
    }
}

// void Game::handleMouseMotion(Sint32 x, Sint32 y){
//     for(auto &id: activeShapes){
//         _shapes[id].setCoords(x, y, true);
//     }
// }

// void Game::handleMouseInput(uint8_t buttonIndex){
//     int x, y;
//     SDL_GetMouseState(&x, &y);
//     switch(buttonIndex){
//         case SDL_BUTTON_LEFT:
//             grabShape(x, y);
//     }
// }

// void Game::grabShape(int x, int y){
//     // grab a shape by mouse x and y by shape z (grap top one)
//     for(auto &shape: _shapes){
//         if(x > shape.xPos && x <= shape.xPos + shape.width && y >= shape.yPos && y <= shape.yPos + shape.height){
//             if(!shape.isGrabbed){
//                 std::cout << "get grabbed" << std::endl;
//                 activeShapes.push_back(shape.id);
//                 shape.setGrabbed(true);
//                 shape.setAnchor(x, y);
//             }
//             continue;
//         }
//         shape.setGrabbed(false);
//     }
// }

// void Game::releaseAllShapes(){
//     for(auto & shape: _shapes){
//         shape.setGrabbed(false);
//     }
//     activeShapes.clear();
// }

// void Game::update(){

//     if(_shapes.size() == 0){
//         makeShape();
//     }
//     for(auto &shape: _shapes){
//         if(shape.moving){
//             shape.setCoords(shape.xPos+(shape.xDelta * shape.velocity), shape.yPos+(shape.yDelta * shape.velocity));
//             checkCollision(shape);
//         }
//     }
// }

// void Game::makeShape(){
//     Shape s(0, 50, 50);
//     // s.setColor({0, 0, 0, 0});
//     Circle c(0, 10, 10);
//     c.setColor({0, 0, 0, 0});
//     c.setCoords(50, 50);
//     _shapes.push_back(c);
// }

void Game::clean()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "game quit" << std::endl;
}

// void Game::checkCollision(Shape& s){
//     if(s.yPos == 0){
//         s.yDelta *= -1;
//     }
//     if(s.xPos == 0){
//         s.xDelta *= -1;
//     }
//     if(s.yPos + s.height == _height){
//         s.yDelta *= -1;
//     }
//     if(s.xPos + s.width == _width){
//         s.xDelta *= -1;
//     }
// }