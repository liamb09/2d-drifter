#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "../include/map.hpp"
#include "../include/line.hpp"
#include "SDL3/SDL.h"

using namespace std;

TerrainMap::TerrainMap () : vertices({}), lines({}) {
    // generate random terrain?
}
TerrainMap::TerrainMap (const vector<vector<SDL_Vertex>> &initVertices, const vector<Line> &initLines) : vertices(initVertices), lines(initLines) {}

vector<vector<SDL_Vertex>> TerrainMap::getVertices () {
    return vertices;
}
void TerrainMap::addTriangleVertices (const vector<vector<float>> &newVertices, const SDL_Color &color) {
    vertices.push_back({SDL_Vertex{SDL_FPoint{newVertices[0][0], newVertices[0][1]}, color}, SDL_Vertex{SDL_FPoint{newVertices[1][0], newVertices[1][1]}, color}, SDL_Vertex{SDL_FPoint{newVertices[2][0], newVertices[2][1]}, color}});
}
void TerrainMap::addQuadVertices(const vector<vector<float>> &newVertices, const SDL_Color &color) {
    vertices.push_back({SDL_Vertex{SDL_FPoint{newVertices[0][0], newVertices[0][1]}, color}, SDL_Vertex{SDL_FPoint{newVertices[1][0], newVertices[1][1]}, color}, SDL_Vertex{SDL_FPoint{newVertices[2][0], newVertices[2][1]}, color}});
    vertices.push_back({SDL_Vertex{SDL_FPoint{newVertices[0][0], newVertices[0][1]}, color}, SDL_Vertex{SDL_FPoint{newVertices[3][0], newVertices[3][1]}, color}, SDL_Vertex{SDL_FPoint{newVertices[2][0], newVertices[2][1]}, color}});
}
void TerrainMap::addQuadOpposites(const vector<vector<float>> &newVertices, const SDL_Color &color) {
    vertices.push_back({SDL_Vertex{SDL_FPoint{newVertices[0][0], newVertices[0][1]}, color}, SDL_Vertex{SDL_FPoint{newVertices[1][0], newVertices[0][1]}, color}, SDL_Vertex{SDL_FPoint{newVertices[1][0], newVertices[1][1]}, color}});
    vertices.push_back({SDL_Vertex{SDL_FPoint{newVertices[0][0], newVertices[0][1]}, color}, SDL_Vertex{SDL_FPoint{newVertices[0][0], newVertices[1][1]}, color}, SDL_Vertex{SDL_FPoint{newVertices[1][0], newVertices[1][1]}, color}});
}

vector<Line> TerrainMap::getLines () {
    return lines;
}
void TerrainMap::addFence (const vector<vector<float>> &newEndpoints, const float &height, const float &barGap, const float &groundGap, const SDL_Color &color) {
    lines.push_back(Line(newEndpoints[0][0], newEndpoints[0][1], newEndpoints[0][0], newEndpoints[0][1] - height, color));
    lines.push_back(Line(newEndpoints[0][0], newEndpoints[0][1] - height, newEndpoints[1][0], newEndpoints[1][1] - height, color));
    lines.push_back(Line(newEndpoints[1][0], newEndpoints[1][1], newEndpoints[1][0], newEndpoints[1][1] - height, color));
    lines.push_back(Line(newEndpoints[0][0], newEndpoints[0][1] - groundGap, newEndpoints[1][0], newEndpoints[1][1] - groundGap, color));
    float fenceLength = sqrt(pow(newEndpoints[0][0] - newEndpoints[1][0], 2) + pow(newEndpoints[0][1] - newEndpoints[1][1], 2));
    // float realBarGap = fenceLength/floor(fenceLength/barGap);
    // cout << realBarGap;
    for (float i = 0; i < fenceLength; i += fenceLength/floor(fenceLength/barGap)) {
        lines.push_back(Line(
            newEndpoints[0][0] + (newEndpoints[1][0]-newEndpoints[0][0])*i/fenceLength, newEndpoints[0][1] + (newEndpoints[1][1]-newEndpoints[0][1])*i/fenceLength - groundGap,
            newEndpoints[0][0] + (newEndpoints[1][0]-newEndpoints[0][0])*i/fenceLength, newEndpoints[0][1] + (newEndpoints[1][1]-newEndpoints[0][1])*i/fenceLength - height,
            color
        ));
    }
}