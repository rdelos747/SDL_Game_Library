## SDL GAME LIBRARY

This library contains wrapper functions/ classes for SDL, aimed at making cross-platform game development in C++ a little bit easier. Note: this project is in the very early stages of development and will probably look very different over the coming months (maybe years???)

# INSTALLATION

1. Install SDL2 and SDL_IMAGE frameworks to your machine. 
2. Include SDL_LIBRARY/FUNCTIONS in your project directory.
3. MAKE!

## MAIN EXAMPLE

FUNCTIONS.h contains all necessary includes and code to get stated. bullet.h and ship.h are custom user-made classed for the specific game. Below is an example of a user's main.cpp:

```c++
// main.cpp
#include "../SDL_LIBRARY/FUNCTIONS/FUNCTIONS.h"
#include "CLASSES/bullet.h"
#include "CLASSES/ship.h"

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

## OTHER EXAMPLES

**CREATING A GAME OBJECT**

```c++
// my_object.h
class my_object:public Object {
public:
	my_object();
	~my_object();
	// below are virtual Object methods a child can override
	void update();
	void keyDown(int k);
	void keyUp(int k);
private:
	// put whatever custom variables here...
}
```

**ADDING A SPRITE TO YOUR OBJECT**
```c++
// my_object.cpp
my_object::my_object {
	// whatever intitialization you like...
	addSprite("path/to/sprite.png");
}
```



# API REFERENCE



**FUNCTIONS.h**
