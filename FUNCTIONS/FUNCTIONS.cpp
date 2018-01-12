#include "FUNCTIONS.h"
//#include "SPRITE.h"

// /////////////////////////////////
// G L O B A L S
// ////////////////////////////////////////////////////////////////

#define PI 3.14159265

//SDL VARS
SDL_Window* WINDOW = NULL;				// Window Object
SDL_Renderer* RENDERER = NULL;			// Render Object

//GAME VARS
int currentID = 0;						// Each Object and Sprite are given a unique ID
std::vector<Object*> OBJECTS;			// Container of Objects
bool GAME_RUNNING = true;				// Game running flag
SDL_Event GAME_EVENT;					// Used for Key events

// /////////////////////////////////
// F U N C T I O N S
// ////////////////////////////////////////////////////////////////

bool SDL_INIT() {
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
	printf("--quitting img\n");
	IMG_Quit();
	printf("--quitting sdl\n");
	SDL_Quit();
}

void GAME_UPDATE() {
	while (GAME_RUNNING) {
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

		// handle object updates
		// handle object renders
		for (int i = 0; i < OBJECTS.size(); i++) {
			if (OBJECTS[i]->isDestroyed() == true) {
				delete OBJECTS[i];
				OBJECTS.erase(OBJECTS.begin() + i);
			} else {
				OBJECTS[i]->update();
				OBJECTS[i]->render();
			}
		}

		SDL_RenderPresent( RENDERER );
	}
}