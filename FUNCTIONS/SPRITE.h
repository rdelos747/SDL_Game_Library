// /////////////////////////////////
// S P R I T E
// ////////////////////////////////////////////////////////////////

#ifndef SPRITE_H
#define SPRITE_H

#include "FUNCTIONS.h"

class Sprite {
public:
	Sprite();
	~Sprite();
	bool loadFromFile(std::string path);

	void freeSprite();

	void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

	int getWidth();
	int getHeight();
	int getID();

private:
	SDL_Texture* texture;
	int ID;
	int spriteWidth;
	int spriteHeight;
};

#include "OBJECT.h"

#endif