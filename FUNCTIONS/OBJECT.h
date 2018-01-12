// /////////////////////////////////
// O B J E C T
// ////////////////////////////////////////////////////////////////

#ifndef OBJECT_H
#define OBJECT_H

#include <vector>
#include "FUNCTIONS.h"
#include "SPRITE.h"

class Sprite;

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
	int getActiveSpriteWidth();
	int getActiveSpriteHeight();
	void selfDestroy();
	bool isDestroyed();

private:
	bool destroy;
	
protected:
	//OBJECT VARS
	int ID;

	//SPRITE VARS
	std::vector<Sprite*> sprites;
	int activeSprite;
	SDL_Point center;
	float x;
	float y;
	int direction;
};

#endif