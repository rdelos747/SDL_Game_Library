// /////////////////////////////////
// T E X T
// ////////////////////////////////////////////////////////////////

#ifndef TEXT_H
#define TEXT_H

#include "ENGINE.h"

class Text {
public:

	Text();
	~Text();
	bool loadFromRenderedText(SDL_Renderer* renderer, std::string textureText, SDL_Color textColor);

	SDL_Texture* texture;
	TTF_Font* font;

	void freeText();
	void freeTextTexture();
	void render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

	int getWidth();
	int getHeight();
	// int getID();
	// int setId(int newID);

private:
	// int ID;
	int textWidth;
	int textHeight;
};

#endif