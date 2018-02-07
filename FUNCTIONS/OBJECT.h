// /////////////////////////////////
// O B J E C T
// ////////////////////////////////////////////////////////////////

#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "FUNCTIONS.h"
#include "SPRITE.h"
#include "TEXT.h"

class Sprite;
class Text;

class Object {
public:
	//functions that get overridden by child
	Object();
	virtual ~Object();
	virtual void update();
	virtual void keyDown(int k);
	virtual void keyUp(int k);

	//not overridden, child must call Object::function()
	void nextSprite();
	void prevSprite();
	void render();
	void addSprite(std::string path);
	//void addFont(std::string path, int size, SDL_Color textColor);
	//void addText(std::string path, int size, SDL_Color textColor, std::string value);
	void setTextFont(std::string path, int size);
	void setTextValue(std::string value, SDL_Color color);
	void setTextValue(int value, SDL_Color color);
	void showSprite();
	void hideSprite();
	bool isVisible();
	int getActiveSpriteWidth();
	int getActiveSpriteHeight();
	void selfDestroy();
	bool isDestroyed();
	int getCollisionLayer();
	bool pointInsideBounds(float pointX, float pointY);

private:
	bool destroy;
	bool visible;
	
protected:
	//OBJECT VARS
	int ID;
	int collisionLayer;

	//SPRITE VARS
	std::vector<Sprite*> sprites;
	Text* text;
	int activeSprite;
	SDL_Point center;
	float x;
	float y;
	int direction;
};

#endif