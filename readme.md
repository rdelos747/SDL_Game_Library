# SDL GAME LIBRARY

This library contains wrapper functions/ classes for SDL, aimed at making cross-platform 2D game development in C++ a little bit easier. 

Note: this project is in the very early stages of development and will probably look very different over the coming months (maybe years???)

## INSTALLATION

1. Install SDL2 and SDL_IMAGE frameworks to your machine. 
2. Include SDL_LIBRARY/FUNCTIONS in your project directory.
3. MAKE!

## SIMPLE EXAMPLE

FUNCTIONS.h contains all necessary includes and code to get stated. 

myObject.h is an example of a custom user-made class for the specific game (where you would put your code :). 

```c++
// main.cpp

#include "path/to/SDL_LIBRARY/FUNCTIONS/FUNCTIONS.h"
#include "path/to/myObject.h"

void setup() {
	Ship* ship = new Ship();
	OBJECTS.push_back(ship);
}

int main(int argc, char* args[]) {
	if (!SDL_INIT()) {
		printf("Failed to init\n");
	} else {
		setup();
		GAME_UPDATE();
	}

	SDL_CLOSE();
	return 0;
}
```

Your main.cpp must #include FUNCTIONS.h, and first call SDL_INIT(), then call GAME_UPDATE(). Finally, you must conclude by calling SDL_CLOSE();

```c++
// myObject.h
#ifndef MYOBJECT_H
#define MYOBJECT_H

#include "path/to/SDL_LIBRARY/FUNCTIONS/FUNCTIONS.h"

class myObject:public Object {
public:
	// Object parent class does much initialization for you (eg setting x = 0, direction = 0, etc...)
	myObject();
	~myObject();
	// below are virtual Object methods the child can override
	void update();
	void keyDown(int k);
	void keyUp(int k);
private:
	// put whatever custom variables here...
};

#endif
```

```c++
// myObject.cpp

#include myObject.h

myObject::myObject() {
	// whatever intitialization you like...
	// for example, add a sprite to your object:
	addSprite("path/to/sprite.png");
}

myObject::~myObject() {
	// Parent Object destructor will be called, so usually you can relax here
}

void myObject::update() {
	// you must override this if you want your custom object to do things on screen.
	// for example:
	x += 5;
}

void myObject::keyDown(int k) {
	//override if you need key down
}

void myObject::keyUp(int k) {
	//override if you need key up
}

// add more custom functions if necessary...
```

# API REFERENCE

## RUNNING THE GAME

**SDL_INIT()**
- Loads up SDL

**SDL_CLOSE()**
- Cleans up user-created objects and sprites. Quits SDL and SDL subsystems.

**GAME_UPDATE()**
- Main game loop. Issues key events, Object updates, and Sprite rendering.

**OBJECTS**
- Type: std::vector<Object*>. Intial [].
- Usage: OBJECTS.push_back(myObjectPointer)
- Custom objects must be added to OBJECTS vector, so that the main loop (GAME_UPDATE) knows which objects to update and render.

## OBJECT CLASS
The Object class is what your custom game classes extend in order to interact properly with SDL and the game loop.

### OBJECT CLASS MEMBER VARS

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

**sprites**
- Type std::vector<Sprite*>. Initial [].
- Vector of Sprite pointers. An Object can contain however many Sprites necessary (perhaps for different visual states, animations, etc...).
- TODO: consider making this private, so child object cannot directly interact with this vector. Child should interact using getters/ setters.

**activeSprite**
- Type int. Initial 0.
- Index of Sprite in sprites that will be rendered by Object's render method. 

### OBJECT CLASS METHODS
- Of course, your custom classes need constructors and destructors, but the parent Object class handles the majority of intialization for you. See 'simple example' for reference.

**update()**
- Type: virtual void. Params: None.
- Child class overrides this. 

**keyDown(int k)**
- Type: virtual void. Params: int.
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
- Type: virtual void. Params: int.
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