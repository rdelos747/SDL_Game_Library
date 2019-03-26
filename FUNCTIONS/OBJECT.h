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
	
	//void addText(std::string path, int size, SDL_Color textColor, std::string value);

	// move these to engine.. then just call linkFont similar to linkSprite..
	//

	// object
	int getID();
	int setID(int newID);
	void selfDestroy();
	bool isDestroyed();
	int getCollisionLayer();
	bool pointInsideBounds(float pointX, float pointY);

	// sprites
	void linkSprite(Sprite* newSprite);
	void nextSprite();
	void prevSprite();
	void showSprite();
	void hideSprite();

	// text
	//void setFont(TTF_Font* font);
	void linkFont(TTF_Font* font);
	void setText(std::string value, SDL_Color color);
	void setText(int value, SDL_Color color);

	// getters for Engine
	//int getActiveSprite();
	void render();

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
	//OBJECT VARS
	// int collisionLayer;

	//SPRITE VARS
	int activeSprite;
	// SDL_Point center;
	// float x;
	// float y;
	// int direction;
};

#endif