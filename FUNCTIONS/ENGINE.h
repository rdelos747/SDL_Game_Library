#ifndef ENGINE_H
#define ENGINE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <map>

#include "engineConfig.h"

#include "SPRITE.h"
#include "OBJECT.h"
#include "TIMER.h"
#include "TEXT.h"
#include "SIZE.h"

#include <vector>
#include <math.h>

#define PI 3.14159265

class Object;
class Sprite;
class Text;

class Engine {
public:
  // GENERAL
  Engine();
  ~Engine();
  bool init(engineConfig newConfig = engineConfig{
    .screenWidth = 640,
    .screenHeight = 480,
    .levelWidth = 640,
    .levelHeight = 480,
    .frameRate = 60,
    .showData = true
  });
  void close();
  bool render();
  void setError(int n);
  bool keyDown(int k);
  bool keyUp(int k);

  // OBJECTS
  int addObject(Object* newObject);
  int destroyObject(Object* obj);

  // COLLISION
  Object* collisionAtOffset(Object* object, float offsetX, float offsetY, int layer);
  bool pointInRect(float pointX, float pointY, float rectX, float rectY, float rectW, float rectH);

  // SPRITES
  void addSprite(std::string key, std::string path);
  Size getSpriteSize(std::string key);

  // CAMERA
  void updateCamera(float newX, float newY);

  // SCREEN
  Size getScreenSize();

  // FONTS
  TTF_Font* addFont(std::string path, int size);

  // HELPERS
  int randInt(int max);
  int randIntRange(int min, int max);
  bool chance(int max);

  SDL_Renderer* renderer;
  SDL_Rect camera;

private:
  void setCamera();

  int screenWidth;
  int screenHeight;
  int levelWidth;
  int levelHeight;
  int frameRate;
  float ticksPerFrame;
  SDL_Window* window;
  SDL_Event gameEvent;

  Timer fpsTimer;
  Timer capTimer;
  int countedFrames;

  bool gameRunning;
  int currentID;
  int errorFound;
  std::vector<Object*> objects;
  std::map<std::string, Sprite*> sprites;
  std::vector<TTF_Font*> fonts;
  std::vector<int> keysDown;
  std::vector<int> keysUp;

  TTF_Font* dataFont;
  Text* dataText;
  
  void renderObject(Object* object);
};

extern Engine ENGINE;

#endif

// // /////////////////////////////////
// // G L O B A L S
// // ////////////////////////////////////////////////////////////////

// #define PI 3.14159265

// //const int SCREEN_WIDTH = 640;
// //const int SCREEN_HEIGHT = 480;
// extern int SCREEN_WIDTH;
// extern int SCREEN_HEIGHT;
// extern int LEVEL_WIDTH;
// extern int LEVEL_HEIGHT;

// extern SDL_Window* WINDOW;
// extern SDL_Renderer* RENDERER;

// class Object;
// extern int currentID;
// extern int errorFound;
// extern std::vector<Object*> OBJECTS;
// extern SDL_Rect CAMERA;
// //extern int NEXT_ID;

// // /////////////////////////////////
// // F U N C T I O N S
// // ////////////////////////////////////////////////////////////////

// // ENGINE
// bool SDL_INIT(int newScreenWidth = 640, int newScreenHeight = 480, int newLevelWidth = 640, int newLevelHeight = 480);
// void SDL_CLOSE();
// bool GAME_UPDATE();
// void updateCamera();

// // HELPERS
// void UPDATE_CAMERA(float newX, float newY);
// int RAND_INT(int max);
// int RAND_INT_RANGE(int min, int max);

// // COLLISONS
// Object* COLLISION_AT_POINT(float newX, float newY, int layer = 0);

// #endif