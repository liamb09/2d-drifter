#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include "../include/car.hpp"
#include "SDL3/SDL.h"

// TODO: make steering much less sensitive so normal turns are easier

using namespace std;

Car::Car (const float &initSCALE) : SCALE(initSCALE), x(0), y(0), vx(0), vy(0), st_angle(0), ACC(0.01), theta(0), TURN_SPEED(0.01), FRICTION(0.99), LENGTH(4.21*initSCALE), WIDTH(1.6*initSCALE), WHEEL_BASE(2.83*initSCALE), TIRE_WIDTH(0.6*initSCALE), TIRE_ANGLE_MAX(0), STEERING_WHEEL_MAX(3*M_PI), STEERING_WHEEL_RETURN(0.95), BODY_COLOR({234, 67, 53, 255}), SECONDARY_COLOR({66, 133, 244, 255}) {}
Car::Car (float initX, float initY, const float &initSCALE, const SDL_Color initBODY_COLOR) : SCALE(initSCALE), x(initX), y(initY), vx(0), vy(0), ACC(0.01), st_angle(0), theta(0), TURN_SPEED(0.01), FRICTION(0.99), LENGTH(4.21*initSCALE), WIDTH(1.6*initSCALE), WHEEL_BASE(2.83*initSCALE), TIRE_WIDTH(0.6*initSCALE), TIRE_ANGLE_MAX(0), STEERING_WHEEL_MAX(3*M_PI), STEERING_WHEEL_RETURN(0.95), BODY_COLOR(initBODY_COLOR), SECONDARY_COLOR({66, 133, 244, 255}) {}
Car::Car (float initX, float initY, float initACC, float initTURN_SPEED, float initFRICTION, float initLENGTH, float initWIDTH, float initWHEEL_BASE, float initTIRE_WIDTH, float initTIRE_ANGLE_MAX, float initSTEERING_WHEEL_MAX, float initSTEERING_WHEEL_RETURN, const float &initSCALE, const SDL_Color initBODY_COLOR, const SDL_Color initSECONDARY_COLOR) : SCALE(initSCALE), x(initX), y(initY), vx(0), vy(0), ACC(initACC), st_angle(0), theta(0), TURN_SPEED(initTURN_SPEED), FRICTION(initFRICTION), WIDTH(initWIDTH*initSCALE), LENGTH(initLENGTH*initSCALE), WHEEL_BASE(initWHEEL_BASE*initSCALE), TIRE_WIDTH(initTIRE_WIDTH*initSCALE), TIRE_ANGLE_MAX(initTIRE_ANGLE_MAX), STEERING_WHEEL_MAX(initSTEERING_WHEEL_MAX), STEERING_WHEEL_RETURN(initSTEERING_WHEEL_RETURN), BODY_COLOR(initBODY_COLOR), SECONDARY_COLOR(initSECONDARY_COLOR) {}

float Car::getX () {
    return x;
}
float Car::getY () {
    return y;
}
float Car::getTheta () {
    return theta;
}
float Car::getSt_angle () {
    return st_angle;
}

void Car::move (bool inputs[], float dt) {
    turnRadius = getTurnRadius();
    centerOfRotation = getCenterOfRotation();
    // currentV = sqrt(vx*vx + vy*vy);

    if (inputs[0]) {
        // vx += ACC*cos(theta);
        // vy += ACC*sin(theta);
        currentV += ACC*dt;
    } else if (inputs[1]) {
        // vx -= ACC*cos(theta);
        // vy -= ACC*sin(theta);
        currentV -= ACC*dt;
    }
    if (inputs[2]) {
        st_angle = max(st_angle - TURN_SPEED*dt, -STEERING_WHEEL_MAX);
    } else if (inputs[3]) {
        st_angle = min(st_angle + TURN_SPEED*dt, STEERING_WHEEL_MAX);
    } else {
        st_angle *= STEERING_WHEEL_RETURN;
        if (abs(st_angle) < 0.1) st_angle = 0;
    }
    
    oldX = x;
    oldY = y;
    if (!isinf(turnRadius)) { // abs(turnRadius) < 1200
        x = centerOfRotation.x - turnRadius * cos(atan2(centerOfRotation.y - oldY, centerOfRotation.x - oldX) + currentV/turnRadius)*abs(turnRadius)/turnRadius;
        y = centerOfRotation.y - turnRadius * sin(atan2(centerOfRotation.y - oldY, centerOfRotation.x - oldX) + currentV/turnRadius)*abs(turnRadius)/turnRadius;
        // cout << "rel_angle: " << (atan2(centerOfRotation.y - oldY, centerOfRotation.x - oldX)) << "  turnRadius: " << turnRadius << "  st_angle: " << st_angle << "  steering_angle=" << getSteeringAngle() << "  theta=" << theta << "  rep_extra=" << (atan2(0, turnRadius)) << "  currentV=" << currentV << "  COR: (" << centerOfRotation.x << ", " << centerOfRotation.y << ")" << "\n";
        theta = atan2(centerOfRotation.y - oldY, centerOfRotation.x - oldX) + currentV/turnRadius - M_PI_2*abs(turnRadius)/turnRadius;
    } else {
        turnRadius = 0;
        x += currentV*cos(theta);
        y += currentV*sin(theta);
        // cout << st_angle << "\n";
    }
    
    currentV *= FRICTION;
    vx *= FRICTION;
    vy *= FRICTION;
}

SDL_FPoint Car::rep (float rx, float ry) {
    return SDL_FPoint{x + sqrt(rx*rx + ry*ry)*cos(theta + atan2(ry, rx)), y + sqrt(rx*rx + ry*ry)*sin(theta + atan2(ry, rx))};
}

SDL_Vertex Car::vtx (float rx, float ry, SDL_Color color) {
    return SDL_Vertex{rep(rx, ry), color};
}
vector<vector<SDL_Vertex>> Car::getVertices () {
    SDL_Color black = {0, 0, 0, 255};
    vector<vector<SDL_Vertex>> result = {
        // Tires
        // Front right tire
        {vtx(LENGTH/4, WIDTH/2, black), vtx(LENGTH/4, WIDTH/2 - TIRE_WIDTH/2, black), vtx(LENGTH*3/8, WIDTH/2 - TIRE_WIDTH/2, black)},
        {vtx(LENGTH/4, WIDTH/2, black), vtx(LENGTH*3/8, WIDTH/2, black), vtx(LENGTH*3/8, WIDTH/2 - TIRE_WIDTH/2, black)},
        // Front left tire
        {vtx(LENGTH/4, -WIDTH/2, black), vtx(LENGTH/4, -WIDTH/2 + TIRE_WIDTH/2, black), vtx(LENGTH*3/8, -WIDTH/2 + TIRE_WIDTH/2, black)},
        {vtx(LENGTH/4, -WIDTH/2, black), vtx(LENGTH*3/8, -WIDTH/2, black), vtx(LENGTH*3/8, -WIDTH/2 + TIRE_WIDTH/2, black)},
        // Back right tire
        {vtx(-LENGTH/4, WIDTH/2, black), vtx(-LENGTH/4, WIDTH/2 - TIRE_WIDTH/2, black), vtx(-LENGTH*3/8, WIDTH/2 - TIRE_WIDTH/2, black)},
        {vtx(-LENGTH/4, WIDTH/2, black), vtx(-LENGTH*3/8, WIDTH/2, black), vtx(-LENGTH*3/8, WIDTH/2 - TIRE_WIDTH/2, black)},
        // Back left tire
        {vtx(-LENGTH/4, -WIDTH/2, black), vtx(-LENGTH/4, -WIDTH/2 + TIRE_WIDTH/2, black), vtx(-LENGTH*3/8, -WIDTH/2 + TIRE_WIDTH/2, black)},
        {vtx(-LENGTH/4, -WIDTH/2, black), vtx(-LENGTH*3/8, -WIDTH/2, black), vtx(-LENGTH*3/8, -WIDTH/2 + TIRE_WIDTH/2, black)},
        // Axles
        {vtx(LENGTH*5/16, WIDTH/2-TIRE_WIDTH/4, black), vtx(LENGTH/4, 0, black), vtx(5*LENGTH/16, -WIDTH/2+TIRE_WIDTH/4, black)},
        {vtx(-LENGTH*5/16, WIDTH/2-TIRE_WIDTH/4, black), vtx(-LENGTH/4, 0, black), vtx(-5*LENGTH/16, -WIDTH/2+TIRE_WIDTH/4, black)},
        // Secondary color accents
        {vtx(3*LENGTH/32, WIDTH/4, SECONDARY_COLOR), vtx(11*LENGTH/32, 0, SECONDARY_COLOR), vtx(3*LENGTH/32, -WIDTH/4, SECONDARY_COLOR)},
        {vtx(-LENGTH/2, 5*WIDTH/16, SECONDARY_COLOR), vtx(11*LENGTH/32, 0, SECONDARY_COLOR), vtx(-LENGTH/2, -5*WIDTH/16, SECONDARY_COLOR)},
        {vtx(3*LENGTH/32, WIDTH/4, SECONDARY_COLOR), vtx(-11*LENGTH/32, 3*WIDTH/16, SECONDARY_COLOR), vtx(3*LENGTH/32, -WIDTH/4, SECONDARY_COLOR)},
        {vtx(3*LENGTH/32, -WIDTH/4, SECONDARY_COLOR), vtx(-11*LENGTH/32, -3*WIDTH/16, SECONDARY_COLOR), vtx(-11*LENGTH/32, 3*WIDTH/16, SECONDARY_COLOR)},
        // Front wing
        {vtx(13*LENGTH/32, WIDTH/2, BODY_COLOR), vtx(15*LENGTH/32, WIDTH/2, BODY_COLOR), vtx(LENGTH/2, 0, BODY_COLOR)},
        {vtx(13*LENGTH/32, WIDTH/2, BODY_COLOR), vtx(14*LENGTH/32, 0, BODY_COLOR), vtx(LENGTH/2, 0, BODY_COLOR)},
        {vtx(13*LENGTH/32, -WIDTH/2, BODY_COLOR), vtx(15*LENGTH/32, -WIDTH/2, BODY_COLOR), vtx(LENGTH/2, 0, BODY_COLOR)},
        {vtx(13*LENGTH/32, -WIDTH/2, BODY_COLOR), vtx(14*LENGTH/32, 0, BODY_COLOR), vtx(LENGTH/2, 0, BODY_COLOR)},
        // Body
        {vtx(5*LENGTH/16, WIDTH/8, BODY_COLOR), vtx(16*LENGTH/32, 0, BODY_COLOR), vtx(5*LENGTH/16, -WIDTH/8, BODY_COLOR)},
        {vtx(15*LENGTH/32, WIDTH/16, BODY_COLOR), vtx(0, 0, BODY_COLOR), vtx(15*LENGTH/32, -WIDTH/16, BODY_COLOR)},
        {vtx(3*LENGTH/32, 3*WIDTH/16, BODY_COLOR), vtx(5*LENGTH/16, WIDTH/8, BODY_COLOR), vtx(5*LENGTH/16, -WIDTH/8, BODY_COLOR)},
        {vtx(3*LENGTH/32, 3*WIDTH/16, BODY_COLOR), vtx(3*LENGTH/32, -3*WIDTH/16, BODY_COLOR), vtx(5*LENGTH/16, -WIDTH/8, BODY_COLOR)},
        {vtx(LENGTH/32, 5*WIDTH/16, BODY_COLOR), vtx(3*LENGTH/32, 3*WIDTH/16, BODY_COLOR), vtx(3*LENGTH/32, -3*WIDTH/16, BODY_COLOR)},
        {vtx(LENGTH/32, 5*WIDTH/16, BODY_COLOR), vtx(LENGTH/32, -5*WIDTH/16, BODY_COLOR), vtx(3*LENGTH/32, -3*WIDTH/16, BODY_COLOR)},
        {vtx(LENGTH/32, 5*WIDTH/16, BODY_COLOR), vtx(-LENGTH/2, 0, BODY_COLOR), vtx(LENGTH/32, -5*WIDTH/16, BODY_COLOR)},
        {vtx(-LENGTH/2, WIDTH/4, BODY_COLOR), vtx(0, 0, BODY_COLOR), vtx(-LENGTH/2, -WIDTH/4, BODY_COLOR)}
    };
    return result;
}

float Car::getSteeringAngle () {
    return st_angle/STEERING_WHEEL_MAX * (M_PI_2-TIRE_ANGLE_MAX);
}
float Car::getTurnRadius () {
    return WHEEL_BASE/sin(getSteeringAngle()) * (TIRE_WIDTH/4);
}
SDL_FPoint Car::getCenterOfRotation () {
    return rep(0, getTurnRadius());
}

string Car::toString () {
    return "(" + to_string(x) + ", " + to_string(y) + ") θ=" + to_string(theta);
}