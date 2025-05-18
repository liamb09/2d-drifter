#include <iostream>
#include <string>
#include <vector>
#include "car.hpp"
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <SDL3/SDL_render.h>

// g++ main.cpp -std=c++11 `pkg-config --libs --cflags sdl3`; ./a.out

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

    // Initialize car
    Car car(WIDTH/2, HEIGHT/2, SCALE, {234, 67, 53, 255});

    Uint64 startTime;
    float dt;
    SDL_Event event;
    int i = 0;
    bool inputs[4] = {false}; // up down left right
    SDL_FPoint centerOfRotation;

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

        // Draw car
        vector<vector<SDL_Vertex>> vertices = car.getVertices();
        for (const vector<SDL_Vertex> &v : vertices) {
            SDL_RenderGeometry(renderer, nullptr, v.data(), 3, nullptr, 0);
        }
        // Draw center of rotation
        centerOfRotation = car.getCenterOfRotation();
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
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