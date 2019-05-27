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

class Object {
public:
	//functions that get overridden by child
	Object(Engine* newParent);
	virtual ~Object();
	virtual void update();
	virtual void keyDown(int k);
	virtual void keyUp(int k);

	//not overridden, child must call Object::function()

	// object
	void selfDestroy();
	bool isDestroyed();
	int getID();
	int setID(int newID);
	int getCollisionLayer();
	bool pointInsideBounds(float pointX, float pointY);

	// for Engine
	void render();

	// sprites
	void linkSprite(Sprite* newSprite);
	void nextSprite();
	void prevSprite();
	void showSprite();
	void hideSprite();

	// text
	void linkFont(TTF_Font* font);
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
	Engine* egPtr;
	Text* text;
	std::vector<Sprite*> sprites;
	
protected:
	int activeSprite;
};

#endif