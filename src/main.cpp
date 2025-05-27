#include <iostream>
#include <string>
#include <vector>
#include "../include/car.hpp"
#include "../include/map.hpp"
#include "../include/line.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

// g++ src/main.cpp src/car.cpp src/map.cpp -std=c++11 `pkg-config --libs --cflags sdl3`; ./a.out

using namespace std;

int main () {
    // Initialize SDL
    SDL_Window *window;
    bool done = false;
    SDL_Init(SDL_INIT_VIDEO);

    const int WIDTH = 1600;
    const int HEIGHT = 900;
    const float SCALE = 20;

    // Create SDL Window
    window = SDL_CreateWindow(
        "2D Drifter",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WIDTH,
        HEIGHT,
        SDL_WINDOW_OPENGL
    );

    // Check that the window was successfully created
    if (window == NULL) {
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED);
    // Check that rendered was successfully created
    if (renderer == NULL) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Initialize map
    TerrainMap map;
    map.addTriangleVertices({{0, 0}, {WIDTH, 0}, {0, HEIGHT}});
    map.addTriangleVertices({{WIDTH, HEIGHT}, {WIDTH, 0}, {0, HEIGHT}}, {250, 213, 165});
    map.addQuadVertices({{200, 200}, {WIDTH-200, 200}, {WIDTH-200, HEIGHT-200}, {200, HEIGHT-200}}, {95, 95, 95});
    map.addQuadOpposites({{500, 300}, {1000, 600}});
    map.addFence({{200, 200}, {0, 200}});
    map.addFence({{200, 200}, {500, 300}});
    map.addFence({{500, 300}, {1000, 600}});

    // Initialize car
    Car car(WIDTH/2, HEIGHT/2, SCALE, {234, 67, 53, 255});

    Uint64 startTime;
    float dt;
    SDL_Event event;
    int i = 0;
    bool inputs[4] = {false}; // up down left right
    SDL_FPoint centerOfRotation;
    vector<vector<SDL_Vertex>> carVertices;
    vector<vector<SDL_Vertex>> mapVertices;
    vector<Line> mapLines;

    while (!done) {
        startTime = SDL_GetTicks();

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                done = true;
            }
            // } else if (event.type == SDL_EVENT_KEY_DOWN) {
            //     switch (event.key.keysym.sym) {
            //         case SDLK_UP:
            //             inputs[0] = true;
            //             break;
            //         case SDLK_DOWN:
            //             inputs[1] = true;
            //             break;
            //         case SDLK_LEFT:
            //             inputs[2] = true;
            //             break;
            //         case SDLK_RIGHT:
            //             inputs[3] = true;
            //             break;
            //         default:
            //             break;
            //     }
            // } else if (event.type == SDL_EVENT_KEY_UP) {
            //     switch (event.key.keysym.sym) {
            //         case SDLK_UP:
            //             inputs[0] = false;
            //             break;
            //         case SDLK_DOWN:
            //             inputs[1] = false;
            //             break;
            //         case SDLK_LEFT:
            //             inputs[2] = false;
            //             break;
            //         case SDLK_RIGHT:
            //             inputs[3] = false;
            //             break;
            //         default:
            //             break;
            //     }
            // }
        }

        // Multiple keys at once
        const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
        if (currentKeyStates[SDL_SCANCODE_UP]) {
            inputs[0] = true;
        } else {
            inputs[0] = false;
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            inputs[1] = true;
        } else {
            inputs[1] = false;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            inputs[2] = true;
        } else {
            inputs[2] = false;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            inputs[3] = true;
        } else {
            inputs[3] = false;
        }

        // Move car
        car.move(inputs, dt);

        // Set background coor
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw map
        mapVertices = map.getVertices();
        for (const vector<SDL_Vertex> &v : mapVertices) {
            SDL_RenderGeometry(renderer, nullptr, v.data(), 3, nullptr, 0);
        }
        mapLines = map.getLines();
        for (const Line &line : mapLines) {
            SDL_SetRenderDrawColor(renderer, line.color.r, line.color.g, line.color.b, line.color.a);
            SDL_RenderLine(renderer, line.p1.x, line.p1.y, line.p2.x, line.p2.y);
        }

        // Draw car
        carVertices = car.getVertices();
        for (const vector<SDL_Vertex> &v : carVertices) {
            SDL_RenderGeometry(renderer, nullptr, v.data(), 3, nullptr, 0);
        }
        // Draw center of rotation
        centerOfRotation = car.getCenterOfRotation();
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);
        SDL_RenderLine(renderer, centerOfRotation.x-10, centerOfRotation.y, centerOfRotation.x+10, centerOfRotation.y);
        SDL_RenderLine(renderer, centerOfRotation.x, centerOfRotation.y-10, centerOfRotation.x, centerOfRotation.y+10);

        // if (i == 10) {
        //     i = 0;
        //     cout << car.toString() << "\n";
        //     cout << centerOfRotation.x << " " << centerOfRotation.y << "\n\n";
        // }
        // i++;

        // Render frame
        SDL_RenderPresent(renderer);

        // Calculate delta time
        dt = (float) (SDL_GetTicks() - startTime);
        // cout << dt << "\n";
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}