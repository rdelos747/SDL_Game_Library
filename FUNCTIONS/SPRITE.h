// /////////////////////////////////
// S P R I T E
// ////////////////////////////////////////////////////////////////

#ifndef SPRITE_H
#define SPRITE_H

#include "ENGINE.h"

class Sprite {
public:
	Sprite();
	~Sprite();
	bool loadFromFile(SDL_Renderer* renderer, std::string path);

	SDL_Texture* texture;

	void freeSprite();

	void render(SDL_Renderer* renderer, 
							int x, 
							int y, 
							SDL_Rect* clip = NULL, 
							double angle = 0.0, 
							SDL_Point* center = NULL, 
							SDL_RendererFlip flip = SDL_FLIP_NONE );

	int getWidth();
	int getHeight();
	int getID();
	int setID(int newID);

private:
	int ID;
	int spriteWidth;
	int spriteHeight;
};

#include "OBJECT.h"

#endif