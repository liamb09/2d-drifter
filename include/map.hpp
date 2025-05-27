#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "../include/line.hpp"
#include "SDL3/SDL.h"

using namespace std;

class TerrainMap {

    vector<vector<SDL_Vertex>> vertices;
    vector<Line> lines;

public:
    TerrainMap ();
    TerrainMap (const vector<vector<SDL_Vertex>> &initVertices, const vector<Line> &initLines);

    vector<vector<SDL_Vertex>> getVertices ();
    void addTriangleVertices (const vector<vector<float>> &newVertices, const SDL_Color &color = {19,133,16});
    void addQuadVertices (const vector<vector<float>> &newVertices, const SDL_Color &color = {19,133,16}); // input vertices clockwise
    void addQuadOpposites (const vector<vector<float>> &newVertices, const SDL_Color &color = {19,133,16});

    vector<Line> getLines ();
    void addFence (const vector<vector<float>> &newEndpoints, const float &height = 30, const float &barGap = 8, const float &groundGap = 8, const SDL_Color &color = {10,10,10});

};