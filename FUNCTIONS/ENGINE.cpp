#include "ENGINE.h"

// /////////////////////////////////
// G E N E R A L
// ////////////////////////////////////////////////////////////////

Engine::Engine() {
	// screenWidth = -1;
	// screenHeight = -1;
	// levelWidth = -1;
	// levelHeight = -1;
	// camera = NULL;
	// window = NULL;
	// renderer = NULL;
}

Engine::~Engine() {}

bool Engine::init(int newScreenWidth, int newScreenHeight, int newLevelWidth, int newLevelHeight, int newFrameRate, bool showData) {
	errorFound = 0;
	gameRunning = true;
	dataFont = NULL;

	screenWidth = newScreenWidth;
	screenHeight = newScreenHeight;
	levelWidth = newLevelWidth;
	levelHeight = newLevelHeight;
	frameRate = newFrameRate;
	ticksPerFrame = 1000 / frameRate;
	camera = {0, 0, screenWidth, screenHeight};
	printf("ticks per frame %f\n", ticksPerFrame);
	//Initialization flag
	bool success = true;
	printf("=============================\n");
	printf("STARTING UP LIBRARY\n");

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	} else {
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf("Warning: Linear texture filtering not enabled!");
		}

		//Create window
		window = SDL_CreateWindow("SDL GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_SHOWN);
		if( window == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		} else {
			//Create vsynced renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if( renderer == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			} else {
				//Initialize renderer color (black)
				SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
				if( TTF_Init() == -1 ) {
        	printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
          success = false;
        }
			}
		}
	}

	if (success) {
		dataFont = addFont("../SDL_LIBRARY/data.ttf", 12);
		dataText = new Text;
		dataText->font = dataFont;
	}

	countedFrames = 0;
  fpsTimer.start();
	return success;
}

void Engine::close() {
	printf("=============================\n");
	printf("CLOSING LIBRARY\n");
	printf("--deleting objects: %lu\n", objects.size());
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
	objects.clear();

	printf("--deleting sprites: %lu\n", sprites.size());
	for(std::map<std::string, Sprite*>::iterator it = sprites.begin(); it != sprites.end(); it++) {
		delete it->second;
	}
	sprites.clear();

	// clear data text
	dataText->freeText();
	dataText = NULL;

	printf("--deleting fonts: %lu\n", fonts.size());
	for (int i = 0; i < fonts.size(); i++) {
		TTF_CloseFont(fonts[i]);
	}
	fonts.clear();

	printf("--destroying renderer\n");
	SDL_DestroyRenderer( renderer );
	printf("--destroying window\n");
	SDL_DestroyWindow( window );
	window = NULL;
	renderer = NULL;

	//Quit SDL subsystems
	printf("--quitting ttf\n");
	TTF_Quit();
	printf("--quitting img\n");
	IMG_Quit();
	printf("--quitting sdl\n");
	SDL_Quit();
}

bool Engine::keyDown(int k) {
	// while(SDL_PollEvent(&gameEvent) != 0) {
	// 	if (gameEvent.type == SDL_KEYDOWN && gameEvent.key.repeat == 0 && gameEvent.key.keysym.sym == k) {
	// 		return true;
	// 	}
	// }
	// return false;
	for (int i = 0; i < keysDown.size(); i++) {
		if (keysDown[i] == k) {
			return true;
		}
	}
	return false;
}

bool Engine::keyUp(int k) {
	// while(SDL_PollEvent(&gameEvent) != 0) {
	// 	if (gameEvent.type == SDL_KEYUP && gameEvent.key.repeat == 0 && gameEvent.key.keysym.sym == k) {
	// 		return true;
	// 	}
	// }
	// return false;
	for (int i = 0; i < keysUp.size(); i++) {
		if (keysUp[i] == k) {
			return true;
		}
	}
	return false;
}

bool Engine::render() {
	if (gameRunning) {
		capTimer.start();
		keysDown.clear();
		keysUp.clear();
		while(SDL_PollEvent(&gameEvent) != 0) {
			// start fps

			if (gameEvent.type == SDL_QUIT) {
				printf("quitting\n");
				gameRunning = false;
			} else if (gameEvent.type == SDL_KEYDOWN && gameEvent.key.keysym.sym == SDLK_ESCAPE) {
				gameRunning = false;
			}
			// handle object keys
			if (gameEvent.type == SDL_KEYDOWN && gameEvent.key.repeat == 0) {
				// for (int i = 0; i < objects.size(); i++) {
				// 	objects[i]->keyDown(gameEvent.key.keysym.sym);
				// }
				keysDown.push_back(gameEvent.key.keysym.sym);
			} else if (gameEvent.type == SDL_KEYUP && gameEvent.key.repeat == 0) {
				// for (int i = 0; i < objects.size(); i++) {
				// 	objects[i]->keyUp(gameEvent.key.keysym.sym);
				// }
				keysUp.push_back(gameEvent.key.keysym.sym);
			}
		}

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);

		/*
			Change render order
			1. user input
			2. update objects that get input
			3. other objects
			4. rendering all objects
		*/

		// Perhaps check for errors here too?

		// handle object destroy
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i]->isDestroyed() == true) {
				delete objects[i];
				objects.erase(objects.begin() + i);
			}
		}

		setCamera();

		// Check for errors before rendering
		if (errorFound != 0) {
			return false;
		}

		//handle object renders
		for (int i = 0; i < objects.size(); i++) {
			renderObject(objects[i]);
		}

		// calculate fps
		float avgFPS = countedFrames / (fpsTimer.getTicks() / 1000.f);
		if (avgFPS > 2000000) {
			avgFPS = 0;
		}
		
		dataText->loadFromRenderedText(renderer, "hey", {255, 255, 255});
		dataText->render(renderer, 10, 10, NULL, 0, NULL);

		SDL_RenderPresent(renderer);
		

		//printf("w: %d\n", dataText->getWidth());

		++countedFrames;
		//If frame finished early
		int frameTicks = capTimer.getTicks();
		if (frameTicks < ticksPerFrame) {
				//Wait remaining time
				SDL_Delay(ticksPerFrame - frameTicks);
		}

		return true;
	} else {
		return false;
	}
}

void Engine::setError(int n) {
	errorFound = n;
}

// /////////////////////////////////
// O B J E C T S
// ////////////////////////////////////////////////////////////////

int Engine::addObject(Object* newObject) {
	newObject->setID(currentID++);
	objects.push_back(newObject);
	return newObject->getID();
}

void Engine::renderObject(Object* object) {
	if (object->visible == true) {
		float renderX = object->x - object->center.x;
		float renderY = object->y - object->center.y;

		std::string spriteKey = object->getSprite();

		if(spriteKey != "") {
			std::map<std::string, Sprite*>::iterator it = sprites.find(spriteKey);
			if(it != sprites.end()) {
				Sprite* sprite = it->second;
				if (sprite->texture != NULL) {
					sprite->render(ENGINE.renderer, renderX - ENGINE.camera.x, renderY - ENGINE.camera.y, NULL, object->direction, NULL);
				}
			}
		}

		Text* text = object->getText();
		if (text != NULL && text->texture != NULL && text->font != NULL) {
			text->render(ENGINE.renderer, object->x - ENGINE.camera.x, object->y - ENGINE.camera.y, NULL, object->direction, NULL);
		}
	}	
}

Object* Engine::collisionAtOffset(Object* object, float offsetX, float offsetY, int layer) {
	Object* foundObject = NULL;
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i] != object && objects[i]->getCollisionLayer() == layer && objects[i]->visible == true) {
			if (objects[i]->isCollidingWithObject(object, offsetX, offsetY) == true) {
				//printf("collide\n");
				foundObject = objects[i];
			}
		}
	}
	return foundObject;
}

bool Engine::pointInRect(float pointX, float pointY, float rectX, float rectY, float rectW, float rectH) {
	return
		pointX >= rectX &&
		pointX <= rectX + rectW &&
		pointY >= rectY &&
		pointY <= rectY + rectH;
}

// /////////////////////////////////
// S P R I T E S
// ////////////////////////////////////////////////////////////////

void Engine::addSprite(std::string key, std::string path) {
	Sprite* s = new Sprite(key, path);
	if(s->texture == NULL) {
		printf("Could not load sprite\n");
		errorFound = 1;
	} else {
		sprites.insert(std::pair<std::string, Sprite*>(key, s));
	}
}

Size Engine::getSpriteSize(std::string key) {
	Size s = {0, 0};
	std::map<std::string, Sprite*>::iterator it = sprites.find(key);
	if(it != sprites.end()) {
		Sprite* sprite = it->second;
		if (sprite->texture != NULL) {
			s.w = sprite->getWidth();
			s.h = sprite->getHeight();
		}
	}
	return s;
}

// /////////////////////////////////
// F O N T S
// ////////////////////////////////////////////////////////////////

TTF_Font* Engine::addFont(std::string path, int size) {
	TTF_Font* font = TTF_OpenFont(path.c_str(), size);
	if (font == NULL) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		errorFound = 1;
	}
	else {
		fonts.push_back(font);
	}

	return font;
}

// /////////////////////////////////
// C A M E R A
// ////////////////////////////////////////////////////////////////

// public
void Engine::updateCamera(float newX, float newY) {
	camera.x = newX;
	camera.y = newY;
}

// private
void Engine::setCamera() {
	if( camera.x < 0 ) { 
		camera.x = 0;
	}
	if( camera.y < 0 ) {
		camera.y = 0;
	}
	if( camera.x > levelWidth - camera.w ) {
		camera.x = levelWidth - camera.w;
	}
	if( camera.y > levelHeight - camera.h ) {
		camera.y = levelHeight - camera.h;
	}
}

// /////////////////////////////////
// S C R E E N
// ////////////////////////////////////////////////////////////////
Size Engine::getScreenSize() {
	return { screenWidth, screenHeight };
}

// /////////////////////////////////
// H E L P E R S
// ////////////////////////////////////////////////////////////////
int Engine::randInt(int max) {
	return rand() % (max + 1);
}

int Engine::randIntRange(int min, int max) {
	return rand() % (max - 1) + min;
}

bool Engine::chance(int max) {
	return randInt(99) < max;
}

Engine ENGINE;

// #include "FUNCTIONS.h"
// //#include "SPRITE.h"

// // /////////////////////////////////
// // G L O B A L S
// // ////////////////////////////////////////////////////////////////

// #define PI 3.14159265

// //SDL VARS
// SDL_Window* WINDOW = NULL;				// Window Object
// SDL_Renderer* RENDERER = NULL;			// Render Object

// int SCREEN_WIDTH = 0;
// int SCREEN_HEIGHT = 0;
// int LEVEL_WIDTH = 0;
// int LEVEL_HEIGHT = 0;

// //GAME VARS
// int currentID = 0;						// Each Object and Sprite are given a unique ID
// int errorFound = 0;						// 0 if no errors, perhaps assign number to different errors
// std::vector<Object*> OBJECTS;			// Container of Objects
// SDL_Rect CAMERA;
// bool GAME_RUNNING = true;				// Game running flag
// SDL_Event GAME_EVENT;					// Used for Key events

// // /////////////////////////////////
// // F U N C T I O N S
// // ////////////////////////////////////////////////////////////////

// bool SDL_INIT(int newScreenWidth, int newScreenHeight, int newLevelWidth, int newLevelHeight) {
// 	SCREEN_WIDTH = newScreenWidth;
// 	SCREEN_HEIGHT = newScreenHeight;
// 	LEVEL_WIDTH = newLevelWidth;
// 	LEVEL_HEIGHT = newLevelHeight;
// 	CAMERA = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
// 	//Initialization flag
// 	bool success = true;
// 	printf("=============================\n");
// 	printf("STARTING UP LIBRARY\n");

// 	//Initialize SDL
// 	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
// 		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
// 		success = false;
// 	} else {
// 		//Set texture filtering to linear
// 		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
// 			printf( "Warning: Linear texture filtering not enabled!" );
// 		}

// 		//Create window
// 		WINDOW = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
// 		if( WINDOW == NULL ) {
// 			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
// 			success = false;
// 		} else {
// 			//Create vsynced renderer for window
// 			RENDERER = SDL_CreateRenderer( WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
// 			if( RENDERER == NULL ) {
// 				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
// 				success = false;
// 			} else {
// 				//Initialize renderer color (black)
// 				SDL_SetRenderDrawColor(RENDERER, 0xFF, 0xFF, 0xFF, 0xFF);

// 				//Initialize PNG loading
// 				int imgFlags = IMG_INIT_PNG;
// 				if( !( IMG_Init( imgFlags ) & imgFlags ) ) {
// 					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
// 					success = false;
// 				}
// 				if( TTF_Init() == -1 ) {
//                     printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
//                     success = false;
//                 }
// 			}
// 		}
// 	}
// 	return success;
// }

// void SDL_CLOSE() {
// 	printf("=============================\n");
// 	printf("CLOSING LIBRARY\n");
// 	printf("--deleting objects: %lu\n", OBJECTS.size());
// 	for (int i = 0; i < OBJECTS.size(); i++) {
// 		//printf("%p\n", OBJECTS[i]);
// 		delete OBJECTS[i];
// 	}
// 	OBJECTS.clear();

// 	printf("--destroying renderer\n");
// 	SDL_DestroyRenderer( RENDERER );
// 	printf("--destroying window\n");
// 	SDL_DestroyWindow( WINDOW );
// 	WINDOW = NULL;
// 	RENDERER = NULL;

// 	//Quit SDL subsystems
// 	printf("--quitting ttf\n");
// 	TTF_Quit();
// 	printf("--quitting img\n");
// 	IMG_Quit();
// 	printf("--quitting sdl\n");
// 	SDL_Quit();
// }

// bool GAME_UPDATE() {
// 	if (GAME_RUNNING) {
// 		while(SDL_PollEvent(&GAME_EVENT) != 0) {
// 			if (GAME_EVENT.type == SDL_QUIT) {
// 				GAME_RUNNING = false;
// 			} else if (GAME_EVENT.type == SDL_KEYDOWN && GAME_EVENT.key.keysym.sym == SDLK_ESCAPE) {
// 				GAME_RUNNING = false;
// 			}
// 			// handle object keys
// 			if (GAME_EVENT.type == SDL_KEYDOWN && GAME_EVENT.key.repeat == 0) {
// 				for (int i = 0; i < OBJECTS.size(); i++) {
// 					OBJECTS[i]->keyDown(GAME_EVENT.key.keysym.sym);
// 				}
// 			} else if (GAME_EVENT.type == SDL_KEYUP && GAME_EVENT.key.repeat == 0) {
// 				for (int i = 0; i < OBJECTS.size(); i++) {
// 					OBJECTS[i]->keyUp(GAME_EVENT.key.keysym.sym);
// 				}
// 			}
// 		}

// 		SDL_SetRenderDrawColor(RENDERER, 0x00, 0x00, 0x00, 0xFF);
// 		SDL_RenderClear(RENDERER);

// 		/*
// 			Change render order
// 			1. user input
// 			2. update objects that get input
// 			3. other objects
// 			4. rendering all objects
// 		*/

// 		// Perhaps check for errors here too?

// 		// handle object updates
// 		for (int i = 0; i < OBJECTS.size(); i++) {
// 			if (OBJECTS[i]->isDestroyed() == true) {
// 				delete OBJECTS[i];
// 				OBJECTS.erase(OBJECTS.begin() + i);
// 			} else {
// 				OBJECTS[i]->update();
// 			}
// 		}

// 		updateCamera();

// 		// Check for errors before rendering
// 		if (errorFound != 0) {
// 			return false;
// 		}

// 		// handle object renders
// 		for (int i = 0; i < OBJECTS.size(); i++) {
// 			OBJECTS[i]->render();
// 		}

// 		SDL_RenderPresent( RENDERER );
// 		return true;
// 	} else {
// 		return false;
// 	}
// }

// void updateCamera() {
// 	if( CAMERA.x < 0 ) { 
// 		CAMERA.x = 0;
// 	}
// 	if( CAMERA.y < 0 ) {
// 		CAMERA.y = 0;
// 	}
// 	if( CAMERA.x > LEVEL_WIDTH - CAMERA.w ) {
// 		CAMERA.x = LEVEL_WIDTH - CAMERA.w;
// 	}
// 	if( CAMERA.y > LEVEL_HEIGHT - CAMERA.h ) {
// 		CAMERA.y = LEVEL_HEIGHT - CAMERA.h;
// 	}
// }

// void UPDATE_CAMERA(float newX, float newY) {
// 	CAMERA.x = newX;
// 	CAMERA.y = newY;
// }

// int RAND_INT(int max) {
// 	return rand() % (max + 1);
// }

// int RAND_INT_RANGE(int min, int max) {
// 	return rand() % (max - 1) + min;
// }


// Object* COLLISION_AT_POINT(float newX, float newY, int layer) {
// 	Object* foundObject = NULL;
// 	for (int i = 0; i < OBJECTS.size(); i++) {
// 		if (OBJECTS[i]->getCollisionLayer() == layer && OBJECTS[i]->isVisible()) {
// 			if (OBJECTS[i]->pointInsideBounds(newX, newY) == true) {
// 				//printf("collide\n");
// 				foundObject = OBJECTS[i];
// 			}
// 		}
// 	}
// 	return foundObject;
// }