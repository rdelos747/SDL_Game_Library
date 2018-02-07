#include "FUNCTIONS.h"
//#include "SPRITE.h"

// /////////////////////////////////
// G L O B A L S
// ////////////////////////////////////////////////////////////////

#define PI 3.14159265

//SDL VARS
SDL_Window* WINDOW = NULL;				// Window Object
SDL_Renderer* RENDERER = NULL;			// Render Object

int SCREEN_WIDTH = 0;
int SCREEN_HEIGHT = 0;
int LEVEL_WIDTH = 0;
int LEVEL_HEIGHT = 0;

//GAME VARS
int currentID = 0;						// Each Object and Sprite are given a unique ID
int errorFound = 0;						// 0 if no errors, perhaps assign number to different errors
std::vector<Object*> OBJECTS;			// Container of Objects
SDL_Rect CAMERA;
bool GAME_RUNNING = true;				// Game running flag
SDL_Event GAME_EVENT;					// Used for Key events

// /////////////////////////////////
// F U N C T I O N S
// ////////////////////////////////////////////////////////////////

bool SDL_INIT(int newScreenWidth, int newScreenHeight, int newLevelWidth, int newLevelHeight) {
	SCREEN_WIDTH = newScreenWidth;
	SCREEN_HEIGHT = newScreenHeight;
	LEVEL_WIDTH = newLevelWidth;
	LEVEL_HEIGHT = newLevelHeight;
	CAMERA = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	//Initialization flag
	bool success = true;
	printf("=============================\n");
	printf("STARTING UP LIBRARY\n");

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	} else {
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) ) {
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		WINDOW = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( WINDOW == NULL ) {
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		} else {
			//Create vsynced renderer for window
			RENDERER = SDL_CreateRenderer( WINDOW, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );
			if( RENDERER == NULL ) {
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			} else {
				//Initialize renderer color (black)
				SDL_SetRenderDrawColor(RENDERER, 0xFF, 0xFF, 0xFF, 0xFF);

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
	return success;
}

void SDL_CLOSE() {
	printf("=============================\n");
	printf("CLOSING LIBRARY\n");
	printf("--deleting objects: %lu\n", OBJECTS.size());
	for (int i = 0; i < OBJECTS.size(); i++) {
		//printf("%p\n", OBJECTS[i]);
		delete OBJECTS[i];
	}
	OBJECTS.clear();

	printf("--destroying renderer\n");
	SDL_DestroyRenderer( RENDERER );
	printf("--destroying window\n");
	SDL_DestroyWindow( WINDOW );
	WINDOW = NULL;
	RENDERER = NULL;

	//Quit SDL subsystems
	printf("--quitting ttf\n");
	TTF_Quit();
	printf("--quitting img\n");
	IMG_Quit();
	printf("--quitting sdl\n");
	SDL_Quit();
}

bool GAME_UPDATE() {
	if (GAME_RUNNING) {
		while(SDL_PollEvent(&GAME_EVENT) != 0) {
			if (GAME_EVENT.type == SDL_QUIT) {
				GAME_RUNNING = false;
			} else if (GAME_EVENT.type == SDL_KEYDOWN && GAME_EVENT.key.keysym.sym == SDLK_ESCAPE) {
				GAME_RUNNING = false;
			}
			// handle object keys
			if (GAME_EVENT.type == SDL_KEYDOWN && GAME_EVENT.key.repeat == 0) {
				for (int i = 0; i < OBJECTS.size(); i++) {
					OBJECTS[i]->keyDown(GAME_EVENT.key.keysym.sym);
				}
			} else if (GAME_EVENT.type == SDL_KEYUP && GAME_EVENT.key.repeat == 0) {
				for (int i = 0; i < OBJECTS.size(); i++) {
					OBJECTS[i]->keyUp(GAME_EVENT.key.keysym.sym);
				}
			}
		}

		SDL_SetRenderDrawColor(RENDERER, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(RENDERER);

		/*
			Change render order
			1. user input
			2. update objects that get input
			3. other objects
			4. rendering all objects
		*/

		// Perhaps check for errors here too?

		// handle object updates
		for (int i = 0; i < OBJECTS.size(); i++) {
			if (OBJECTS[i]->isDestroyed() == true) {
				delete OBJECTS[i];
				OBJECTS.erase(OBJECTS.begin() + i);
			} else {
				OBJECTS[i]->update();
			}
		}

		updateCamera();

		// Check for errors before rendering
		if (errorFound != 0) {
			return false;
		}

		// handle object renders
		for (int i = 0; i < OBJECTS.size(); i++) {
			OBJECTS[i]->render();
		}

		SDL_RenderPresent( RENDERER );
		return true;
	} else {
		return false;
	}
}

void updateCamera() {
	if( CAMERA.x < 0 ) { 
		CAMERA.x = 0;
	}
	if( CAMERA.y < 0 ) {
		CAMERA.y = 0;
	}
	if( CAMERA.x > LEVEL_WIDTH - CAMERA.w ) {
		CAMERA.x = LEVEL_WIDTH - CAMERA.w;
	}
	if( CAMERA.y > LEVEL_HEIGHT - CAMERA.h ) {
		CAMERA.y = LEVEL_HEIGHT - CAMERA.h;
	}
}

void UPDATE_CAMERA(float newX, float newY) {
	CAMERA.x = newX;
	CAMERA.y = newY;
}

int RAND_INT(int max) {
	return rand() % (max + 1);
}

int RAND_INT_RANGE(int min, int max) {
	return rand() % (max - 1) + min;
}


Object* COLLISION_AT_POINT(float newX, float newY, int layer) {
	Object* foundObject = NULL;
	for (int i = 0; i < OBJECTS.size(); i++) {
		if (OBJECTS[i]->getCollisionLayer() == layer && OBJECTS[i]->isVisible()) {
			if (OBJECTS[i]->pointInsideBounds(newX, newY) == true) {
				//printf("collide\n");
				foundObject = OBJECTS[i];
			}
		}
	}
	return foundObject;
}