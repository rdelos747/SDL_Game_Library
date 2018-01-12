#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <string>

#include "SPRITE.h"
#include "OBJECT.h"

#include <vector>
#include <math.h>

// /////////////////////////////////
// G L O B A L S
// ////////////////////////////////////////////////////////////////

#define PI 3.14159265

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

extern SDL_Window* WINDOW;
extern SDL_Renderer* RENDERER;

class Object;
extern int currentID;
extern std::vector<Object*> OBJECTS;
//extern int NEXT_ID;

// /////////////////////////////////
// F U N C T I O N S
// ////////////////////////////////////////////////////////////////

bool SDL_INIT();
void SDL_CLOSE();
void GAME_UPDATE();

#endif