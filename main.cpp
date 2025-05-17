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

    // Create SDL Window
    window = SDL_CreateWindow(
        "Parametric Fitter",
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
    //        X        Y       ACC  TurnV  FR   L    W
    Car car(WIDTH/2, HEIGHT/2, 0.1, .005, 0.95, 25, 50);

    Uint64 startTime;
    float dt;
    SDL_Event event;
    int i = 0;
    bool inputs[4] = {false}; // up down left right

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

        if (inputs[0] && inputs[2]) {
            cout << "yep" << "\n";
        }

        // Move car
        car.move(inputs, dt);
        if (i == 10) {
            i = 0;
            cout << car.toString() << "\n";
        }

        // Set background coor
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        // Draw car
        vector<vector<SDL_Vertex>> vertices = car.getVertices();
        SDL_SetRenderDrawColor(renderer, 18, 102, 241, 255);
        SDL_RenderGeometry(renderer, nullptr, vertices[0].data(), 3, nullptr, 0);
        SDL_RenderGeometry(renderer, nullptr, vertices[1].data(), 3, nullptr, 0);

        // Render frame
        SDL_RenderPresent(renderer);

        i++;

        // Calculate delta time
        dt = (float) (SDL_GetTicks() - startTime);
        // cout << dt << "\n";
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;

}