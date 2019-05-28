// /////////////////////////////////
// O B J E C T
// ////////////////////////////////////////////////////////////////

#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "ENGINE.h"
#include "SPRITE.h"
#include "TEXT.h"

class Engine;
class Text;
class Sprite;
struct Size;

class Object {
public:
	//functions that get overridden by child
	Object();
	virtual ~Object();
	// virtual void update();
	// virtual void keyDown(int k);
	// virtual void keyUp(int k);

	//not overridden, child must call Object::function()

	// object
	void selfDestroy();
	bool isDestroyed();
	int getID();
	int setID(int newID);
	
	// collision
	int getCollisionLayer();
	bool isCollidingWithObject(Object* object, float offsetX = 0, float offsetY = 0);

	// sprites
	std::string getSprite();
	void setSprite(std::string key);
	void showSprite();
	void hideSprite();
	Size getSpriteSize();

	// text
	void linkFont(TTF_Font* font);
	Text* getText();
	void setText(std::string value, SDL_Color color);
	void setText(int value, SDL_Color color);

	// vars
	int collisionLayer;
	SDL_Point center;
	float x;
	float y;
	int direction;
	bool visible;

private:
	bool destroy;
	int ID;
	//Engine* egPtr;
	Text* text;
	std::string sprite;
};

#endif