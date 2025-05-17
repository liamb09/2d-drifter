#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "car.hpp"
#include "SDL3/SDL.h"

using namespace std;

Car::Car () : x(0), y(0), vx(0), vy(0), ACC(1), theta(0), TURN_SPEED(.1), FRICTION(0.5), WIDTH(100), LENGTH(200) {}
Car::Car (float initX, float initY, float initACC, float initTURN_SPEED, float initFRICTION, float initWIDTH, float initLENGTH) : x(initX), y(initY), vx(0), vy(0), ACC(initACC), theta(0), TURN_SPEED(initTURN_SPEED), FRICTION(initFRICTION), WIDTH(initWIDTH), LENGTH(initLENGTH) {}

float Car::getX () {
    return x;
}
float Car::getY () {
    return y;
}
float Car::getTheta () {
    return theta;
}

void Car::move (bool inputs[], float dt) {
    if (inputs[0]) {
        vx += ACC*cos(theta);
        vy += ACC*sin(theta);
    } else if (inputs[1]) {
        vx -= ACC*cos(theta);
        vy -= ACC*sin(theta);
    }
    if (abs(vx) > 0.001 || abs(vy) > 0.001) {
        if (inputs[2]) {
            theta -= TURN_SPEED*dt;
        } else if (inputs[3]) {
            theta += TURN_SPEED*dt;
        }
    }
    float mv = sqrt(vx*vx + vy*vy); // mv = magnitude of velocity
    if (vx > 0.1 || vy > 0.1) cout << vx << " " << vy << " " << theta << "\n";
    x += vx*dt;
    y += vy*dt;
    vx *= FRICTION;
    vy *= FRICTION;
}

SDL_Vertex Car::rep (float rx, float ry) {
    return SDL_Vertex{x + sqrt(rx*rx + ry*ry)*cos(theta + atan2(ry, rx)), y + sqrt(rx*rx + ry*ry)*sin(theta + atan2(ry, rx))};
}
vector<vector<SDL_Vertex>> Car::getVertices () {
    vector<vector<SDL_Vertex>> result = {
        {rep(LENGTH, WIDTH), rep(-LENGTH, WIDTH), rep(-LENGTH, -WIDTH)},
        {rep(LENGTH, WIDTH), rep(LENGTH, -WIDTH), rep(-LENGTH, -WIDTH)}
    };
    return result;
}

string Car::toString () {
    return "(" + to_string(x) + ", " + to_string(y) + ") θ=" + to_string(theta);
}