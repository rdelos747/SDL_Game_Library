// /////////////////////////////////
// S P R I T E
// ////////////////////////////////////////////////////////////////

#include "SPRITE.h"

Sprite::Sprite(std::string newKey, std::string path) {
	key = newKey;
	texture = NULL;
	spriteWidth = 0;
	spriteHeight = 0;
	loadFromFile(path);
}

Sprite::~Sprite() {
	printf("hi");
	freeSprite();
}

void Sprite::loadFromFile(std::string path) {
	//Get rid of preexisting texture
	freeSprite();

	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	if (loadedSurface == NULL) {
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
	} else {
		//Color key image (black)
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0, 0));

		//Create texture from surface pixels
    	newTexture = SDL_CreateTextureFromSurface(ENGINE.renderer, loadedSurface);
		if (newTexture == NULL) {
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
		} else {
			//Get image dimensions
			spriteWidth = loadedSurface->w;
			spriteHeight = loadedSurface->h;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	//Return success
	texture = newTexture;
}

void Sprite::freeSprite()
{
	//Free texture if it exists
	if( texture != NULL )
	{
		SDL_DestroyTexture( texture );
		texture = NULL;
		spriteWidth = 0;
		spriteHeight = 0;
	}
}

void Sprite::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
	//Set rendering space and render to screen
	SDL_Rect renderQuad = { x, y, spriteWidth, spriteHeight };

	//Set clip rendering dimensions
	if( clip != NULL )
	{
		renderQuad.w = clip->w;
		renderQuad.h = clip->h;
	}

	//Render to screen
	SDL_RenderCopyEx(renderer, texture, clip, &renderQuad, angle, center, flip );
}

int Sprite::getWidth() {
	return spriteWidth;
}

int Sprite::getHeight() {
	return spriteHeight;
}

std::string Sprite::getKey() {
	return key;
}