// /////////////////////////////////
// O B J E C T
// ////////////////////////////////////////////////////////////////

#include "OBJECT.h"

// /////////////////////////////////
// CONSTRUCTOR
// ////////////////////////////////////////////////////////////////

Object::Object() {
	destroy = false;
	ID = currentID++;
	printf(" --OBJECT CONSTRUCTOR %d\n", ID);
	x = 0;
	y = 0;
	direction = 0;
	center.x = 0;
	center.y = 0;
	activeSprite = 0;
	sprites.clear();
}

// /////////////////////////////////
// DESTRUCTOR
// ////////////////////////////////////////////////////////////////

Object::~Object() {
	printf(" --OBJECT DESTRUCTOR %d\n", ID);
	x = 0;
	y = 0;
	ID = 0;
	direction = 0;
	activeSprite = 0;
	center.x = 0;
	center.y = 0;
	for (int i = 0; i < sprites.size(); i++) {
		delete sprites[i];
	}
	sprites.clear();
}

// /////////////////////////////////
// VIRTUAL OVERRIDES
// ////////////////////////////////////////////////////////////////

void Object::update() {}
void Object::keyDown(int k) {}
void Object::keyUp(int k) {}

// /////////////////////////////////
// RENDER & DESTROY (never called by child)
// ////////////////////////////////////////////////////////////////

void Object::render() {
	float renderX = x - center.x;
	float renderY = y - center.y;
	sprites[activeSprite]->render(renderX, renderY, NULL, direction, NULL);
}

// /////////////////////////////////
// DESTROY (called by child)
// ////////////////////////////////////////////////////////////////

void Object::selfDestroy() {
	destroy = true;
}

bool Object::isDestroyed() {
	return destroy;
}

// /////////////////////////////////
// SPRITE FUNCTIONS (called by child)
// ////////////////////////////////////////////////////////////////

void Object::addSprite(std::string path) {
	Sprite* s = new Sprite;
	if(!s->loadFromFile(path)) {
		printf("Could not load sprite\n");
	} else {
		sprites.push_back(s);
	}
}

void Object::nextSprite() {
	activeSprite++;
	if (activeSprite == sprites.size()) {
		activeSprite = 0;
	}
}

void Object::prevSprite() {
	activeSprite--;
	if (activeSprite > 0) {
		activeSprite = sprites.size() - 1;
	}
}

int Object::getActiveSpriteWidth() {
	return sprites[activeSprite]->getWidth();
}

int Object::getActiveSpriteHeight() {
	return sprites[activeSprite]->getHeight();
}