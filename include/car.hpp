#include <iostream>
#include <string>
#include <vector>
#include "SDL3/SDL.h"

using namespace std;

class Car {

    float x, y, oldX, oldY, vx, vy, currentV, theta, st_angle, turnRadius; // st_angle = steering wheel angle
    float ACC, TURN_SPEED, FRICTION;
    float WIDTH, LENGTH, WHEEL_BASE, TIRE_WIDTH, TIRE_ANGLE_MAX, STEERING_WHEEL_MAX, STEERING_WHEEL_RETURN;
    float SCALE;
    SDL_Color BODY_COLOR, SECONDARY_COLOR;
    SDL_FPoint centerOfRotation;

public:
    Car (const float &initSCALE);
    Car (float initX, float initY, const float &initSCALE, const SDL_Color initBODY_COLOR);
    Car (float initX, float initY, float initACC, float initTURN_SPEED, float initFRICTION, float initLENGTH, float initWIDTH, float initWHEEL_BASE, float initTIRE_WIDTH, float initTIRE_ANGLE_MAX, float initSTEERING_WHEEL_MAX, float initSTEERING_WHEEL_RETURN, const float &initSCALE, const SDL_Color initBODY_COLOR, const SDL_Color initSECONDARY_COLOR);

    float getX ();
    float getY ();
    float getTheta ();
    float getSt_angle ();

    void move (bool inputs[], float dt);

    SDL_FPoint rep (float rx, float ry); // relative point
    SDL_Vertex vtx (float rx, float ry,  SDL_Color color);
    vector<vector<SDL_Vertex>> getVertices ();

    float getSteeringAngle ();
    float getTurnRadius ();
    SDL_FPoint getCenterOfRotation ();
    
    string toString ();
};