# SDL GAME LIBRARY

This library contains wrapper functions/ classes for SDL, aimed at making cross-platform 2D game development in C++ a little bit easier. 

Note: this project is in the very early stages of development and will probably look very different over the coming months (maybe years???)

## INSTALLATION

1. Install SDL2 and SDL_IMAGE frameworks to your machine. (I used brew on osx)
2. Include SDL_LIBRARY/FUNCTIONS/ENGINE.h"in your project directory.
3. MAKE!

## TODOs

- Include should just be `path/to/library/engine.h` instead of that mess with the functions directory.
- Fix up this readme.. everything in the api reference is wrong :)
- Stop rendering offscreen objects (add render padding around screen)

## SIMPLE EXAMPLE

ENGINE.h contains all necessary includes and code to get stated. 

myObject.h is an example of a custom user-made class for the specific game (where you would put your code :). 

```c++
// test.cpp

#include <iostream>
#include "pathto/SDL_LIBRARY/FUNCTIONS/ENGINE.h"

Engine* engine = new Engine();

class Test: public Object {
public:
	Test(Engine* parent)
	:Object(parent) {
		// you can add sprites here by calling parent->addSprite(...)
		// and then call linkSprite(pointerToAboveSprite)
	}


	// update, keyDown, and keyUp are virtuals that the child can override
	void update() {
		x+=1;
	}

	void keyDown(int k) {}
	void keyUp(int k){}
};

class TextTest: public Object {
public:
	TextTest(Engine* parent, TTF_Font* f)
	:Object(parent) {
		x = 10;
		y = 10;
		linkFont(f);
	}

	void update() {
		setText("heyo", {255,255,255});
	}
};


void setup() {
	// my preferred way of adding sprites/ fonts is to declare them all in the setup...
	// you can also do this within each object for better organization (see above)
	Sprite* testSprite = engine->addSprite("path/to/sprite.png");
	TTF_Font* testFont = engine->addFont("./myfont.ttf", 10);

	Test* test = new Test(engine);
	test->linkSprite(testSprite);
	engine->addObject(test);

	TextTest* tt = new TextTest(engine, testFont);
	engine->addObject(tt);
}

int main(int argc, char* args[]) {
	if (!engine->init()) {
		printf("Failed to init\n");
	} else {
		setup();
		bool runGame = true;
		while(runGame) {
			runGame = engine->update();
		}
	}

	engine->close();
	return 0;
}
```

# API REFERENCE

## RUNNING THE GAME

**SDL_INIT(int newScreenWidth = 640, int newScreenHeight = 480, int newLevelWidth = 640, int newLevelHeight = 480)**
- Loads up SDL
- User can change default screen/ level sizes if necessary.

**SDL_CLOSE()**
- Cleans up user-created objects and sprites. Quits SDL and SDL subsystems.

**GAME_UPDATE()**
- Main game loop. Issues key events, Object updates, and Sprite rendering.

**SCREEN_WIDTH**
- Type const int. Initial 640.
- Width of screen.

**SCREEN_HEIGHT**
- Type const int. Initial 480.
- Height of screen.

**LEVEL_WIDTH**
- Type const int. Initial 640.
- Width of level.

**LEVEL_HEIGHT**
- Type const int. Initial 480.
- Height of level.

**OBJECTS**
- Type std::vector<Object*>. Intial [].
- Usage: OBJECTS.push_back(myObjectPointer)
- Custom objects must be added to OBJECTS vector, so that the main loop (GAME_UPDATE) knows which objects to update and render.

**CAMERA**
- Type SDL_Rect. Initial {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT}.
- CAMERA will always be the width/ height of screen.

### GLOBAL FUNCTIONS

**UPDATE_CAMERA(float newX, float newY)**
- Type void. Params float, float.
- Change camera position within level.
```c++
void myObject::update() {
	x++;
	UPDATE_CAMERA(x - (SCREEN_WIDTH / 2), y);
}
```

- Note: CAMERA will not move past the bounds of LEVEL_WIDTH/ LEVEL_HEIGHT.

**RAND_INT(int max)**
- Type int. Params int.
- Returns random int between 0 and max, inclusive.

**RAND_INT_RANGE(int min, int max)**
- Type int. Params int, int.
- Returns random int between min and max, inclusive.

**COLLISION_AT_POINT(float x, float y, int layer)**
- Type Object*. Params float, float, int.
- Returns pointer to object with collisionLayer *layer* found at point x, y. Returns NULL if no object found.
```c++
	Object* foundOBJ = COLLISION_AT_POINT(x, y, 1);
	if (foundOBJ != NULL) {
		customObject* a = (customObject *)foundOBJ;
		a->someChildFunction();
		selfDestroy();
	}
```
- Since COLLISON_AT_POINT returns an Object pointer, it is useful to cast that pointer to a pointer of whatever type it exists as in your game, so you may call child class functions if necessary.

## OBJECT CLASS
The Object class is what your custom game classes extend in order to interact properly with SDL and the game loop.

### OBJECT CLASS MEMBER VARS

**ID**
- Type int (const?). Initial 0.
- Unique ID of parent Object, set during parent construction.

**x**
- Type float. Initial 0.
- The horizontal positioning of the object on the screen

**y**
- Type float. Initial 0.
- The vertical positioning of the object on the screen

**direction**
- Type int. Initial 0.
- The direction the Object is pointing. The Object's active sprite will render facing this direction.
- TODO: Add Sprite::spriteDirection, so that Sprite direction and Object direction are independent

**center**
- Type SDL_Point. Initial 0, 0.
- Center point of Object. Object is rendered at point (x - center.x, y - center.y).

**collisionLayer**
- Type int. Initial 0.
- Used to differentiate different objects for collision detection.

**sprites**
- Type std::vector<Sprite*>. Initial [].
- Vector of Sprite pointers. An Object can contain however many Sprites necessary (perhaps for different visual states, animations, etc...).
- TODO: consider making this private, so child object cannot directly interact with this vector. Child should interact using getters/ setters.

**text**
- Type Text*. Initial NULL.
- Text object used for rendering text to the screen.

**activeSprite**
- Type int. Initial 0.
- Index of Sprite in sprites that will be rendered by Object's render method. 

### OBJECT CLASS METHODS
- Of course, your custom classes need constructors and destructors, but the parent Object class handles the majority of intialization for you. See 'simple example' for reference.

**update()**
- Type virtual void. Params None.
- Child class overrides this. 

**keyDown(int k)**
- Type virtual void. Params int.
- Called from GAME_UPDATE loop.
- Child class overrides this to add key down functionality.
- See SDL manual for list of key codes.
```c++
void myObject::keyDown(int k) {
	switch(k) {
		case SDLK_RIGHT: foo(); break;
		case SDLK_LEFT: bar(); break;
		case SDLK_UP: direction++; break;
		case SDLK_SPACE: x = 6;
	}
}
```

**keyUP(int k)**
- Type virtual void. Params int.
- Called from GAME_UPDATE loop.
- Child class overrides this to add key up functionality.
- See SDL manual for list of key codes.

```c++
void myObject::keyUp(int k) {
	switch(k) {
		case SDLK_RIGHT: bar(); break;
		case SDLK_LEFT: foo(); break;
		case SDLK_UP: direction--; break;
	}
}
```

**selfDestroy()**
Type void. Params none.
- Destroys Object.
```c++
void myObject::update() {
	if (x > SCREEN_WIDTH) {
		selfDestroy();
	}
}
```

**addSprite(std::string path)**
Type void. Params std::string
- Adds Sprite to sprites vector.
- TODO: Perhaps make this return t/f incase adding sprite fails.

**nextSprite()**
Type void. Params none.
- Increments activeSprite by 1. This causes next Sprite in sprites vector to be rendered.
- If activeSprite is at end of sprites vector, activeSprite will reset to 0 if nextSprite() is called.

**prevSprite()**
Type void. Params none.
- Decrements activeSprite by 1. This causes previous Sprite in sprites vector to be rendered.
- If activeSprite is at 0, activeSprite will set to sprites.size() - 1 if prevSprite() is called.

**getActiveSpriteWidth()**
Type int. Params none.
- Returns width of active sprite.

**getActiveSpriteHeight()**
Type int. Params none.
- Returns height of active sprite.

**showSprite()**
Type void. Params none.
- Sets Object sprite to visible.

**hideSprite()**
Type void. Params none.
- Sets Object sprite to invisible. Invisible sprites are not considered for collision detection.

**isVisible()**
Type bool. Params none.
- Returns whether or not Object sprites are visible.

**getCollisionLayer()**
Type void. Params none.
- Returns collision layer of Object.

**setTextFont(std::string path, int size)**
Type void. Params string, int.
- Initializes Object's Text object with Font. Must be called before other text methods. Font must be ttf.

```c++
setTextFont("ASSETS/myFont.ttf", 20);
setTextValue("0", {255, 255, 255});
```

**setTextValue(std::string value/ int value, SDL_Color color)**
Type void. Params string/ int, SDL_Color.
- Sets text to be shown. Value can be either a string or int.
