# SDL GAME LIBRARY [Depricated]

This library contains wrapper functions/ classes for SDL, aimed at making cross-platform 2D game development in C++ a little bit easier.

Note: this project is in the very early stages of development and will probably look very different over the coming months (maybe years???)

NOTE: I have since moved to FNA (c#) and Pico8 for game development. I still keep this repo around for reference, but I don't use it and neither should you :)

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
