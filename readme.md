## SDL GAME LIBRARY

This library contains wrapper functions/ classes for SDL, aimed at making cross-platform game development in C++ a little bit easier.

## EXAMPLE

FUNCTIONS.h contains all necessary includes and code to get stated. 
bullet.h and ship.h are custom user-made classed for the specific game.

```c++
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