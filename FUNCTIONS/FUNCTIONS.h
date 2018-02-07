#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <SDL2_ttf/SDL_ttf.h>
#include <stdio.h>
#include <string>

#include "SPRITE.h"
#include "TEXT.h"
#include "OBJECT.h"

#include <vector>
#include <math.h>

// /////////////////////////////////
// G L O B A L S
// ////////////////////////////////////////////////////////////////

#define PI 3.14159265

//const int SCREEN_WIDTH = 640;
//const int SCREEN_HEIGHT = 480;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

extern SDL_Window* WINDOW;
extern SDL_Renderer* RENDERER;

class Object;
extern int currentID;
extern int errorFound;
extern std::vector<Object*> OBJECTS;
//extern int NEXT_ID;

// /////////////////////////////////
// F U N C T I O N S
// ////////////////////////////////////////////////////////////////

// ENGINE
bool SDL_INIT(int newScreenWidth = 640, int newScreenHeight = 480);
void SDL_CLOSE();
bool GAME_UPDATE();

// HELPERS
int RAND_INT(int max);
int RAND_INT_RANGE(int min, int max);

// COLLISONS
Object* COLLISION_AT_POINT(float newX, float newY, int layer = 0);

#endif