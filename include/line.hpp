#include <iostream>
#include <SDL3/SDL.h>
#pragma once

struct Line {
    SDL_FPoint p1;
    SDL_FPoint p2;
    SDL_Color color;

    Line (const float &x1, const float &y1, const float &x2, const float &y2, const SDL_Color &initColor) : p1(SDL_FPoint{x1, y1}), p2(SDL_FPoint{x2, y2}), color(initColor) {}
};