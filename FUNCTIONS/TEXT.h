// /////////////////////////////////
// T E X T
// ////////////////////////////////////////////////////////////////

#ifndef TEXT_H
#define TEXT_H

#include "FUNCTIONS.h"

class Text {
public:

	Text();
	~Text();
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

	SDL_Texture* texture;
	TTF_Font* font;

	void freeText();
	void freeTextTexture();
	void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );

	int getWidth();
	int getHeight();
	int getID();

private:
	int ID;
	int textWidth;
	int textHeight;
};

#endif