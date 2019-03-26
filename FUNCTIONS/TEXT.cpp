// /////////////////////////////////
// T E X T
// ////////////////////////////////////////////////////////////////

#include "SPRITE.h"

Text::Text() {
	// ID = -1;
	texture = NULL;
	font = NULL;
	textWidth = 0;
	textHeight = 0;
}

Text::~Text() {
	freeText();
	// ID = 0;
}

bool Text::loadFromRenderedText(SDL_Renderer* renderer, std::string textureText, SDL_Color textColor)
{
    //Get rid of preexisting texture
    freeTextTexture();

    //Render text surface
    SDL_Texture* newTexture = NULL;
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
    if(textSurface == NULL) {
        printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
    } else {
        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        if( newTexture == NULL ) {
            printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
        } else {
            //Get image dimensions
            textWidth = textSurface->w;
            textHeight = textSurface->h;
        }

        //Get rid of old surface
        SDL_FreeSurface(textSurface);
    }
    
    //Return success
    texture = newTexture;
    return texture != NULL;
}

void Text::freeText() {
	//Free texture if it exists
	//printf("   --in sprite free ID: %d\n", ID);
	if( texture != NULL ) {
		//printf("trying to free sprite?\n");
		SDL_DestroyTexture( texture );
		texture = NULL;
		textWidth = 0;
		textHeight = 0;
		//printf("   --freeing sprite ID: %d\n", ID);
		//TTF_CloseFont( font ); do this in engine
    	font = NULL;
		// ID = 0;
	}
}

void Text::freeTextTexture() {
    if (texture != NULL) {
        SDL_DestroyTexture( texture );
        texture = NULL;
        textWidth = 0;
        textHeight = 0;
    }
}

void Text::render(SDL_Renderer* renderer, int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip )
{
    //Set rendering space and render to screen
    SDL_Rect renderQuad = { x, y, textWidth, textHeight };

    //Set clip rendering dimensions
    if( clip != NULL )
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    //Render to screen
    SDL_RenderCopyEx( renderer, texture, clip, &renderQuad, angle, center, flip );
}

int Text::getWidth() {
	return textWidth;
}

int Text::getHeight() {
	return textHeight;
}

// int Text::getID() {
// 	return ID;
// }

// int Text::setID(int newID) {
// 	if (ID == -1) {
// 		ID = newID;
// 	}
// 	else {
// 		printf("Text ID already set as %d\n", ID);
// 	}

// 	return ID;
// }