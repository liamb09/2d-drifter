#include <iostream>
#include <string>
#include <vector>
#include "SDL3/SDL.h"

using namespace std;

class Car {

    float x, y, vx, vy, theta;
    float WIDTH, LENGTH, ACC, TURN_SPEED, FRICTION;

public:
    Car (); 
    Car (float initX, float initY, float initACC, float initTURN_SPEED, float initFRICTION, float initWIDTH, float initLENGTH);

    float getX ();
    float getY ();
    float getTheta ();

    void move (bool inputs[], float dt);

    SDL_Vertex rep (float rx, float ry); // relative point
    vector<vector<SDL_Vertex>> getVertices ();
    
    string toString ();
};