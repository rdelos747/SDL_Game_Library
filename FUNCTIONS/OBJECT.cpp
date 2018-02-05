// /////////////////////////////////
// O B J E C T
// ////////////////////////////////////////////////////////////////

#include "OBJECT.h"

// /////////////////////////////////
// CONSTRUCTOR
// ////////////////////////////////////////////////////////////////

Object::Object() : sprites() {
	//sprites.reserve(10);
	destroy = false;
	ID = currentID++;
	collisionLayer = 0;
	//printf(" --OBJECT CONSTRUCTOR %d\n", ID);
	x = 0;
	y = 0;
	direction = 0;
	center.x = 0;
	center.y = 0;
	activeSprite = 0;
	//sprites.clear();
}

// /////////////////////////////////
// DESTRUCTOR
// ////////////////////////////////////////////////////////////////

Object::~Object() {
	//printf(" --OBJECT DESTRUCTOR %d\n", ID);
	x = 0;
	y = 0;
	ID = 0;
	collisionLayer = 0;
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
	//printf("rendering obj %d %d\n", ID, sprites[activeSprite]->getID());
	//printf("rendering obj\n");
	//printf(" %d %d\n", ID, sprites[activeSprite]->getID());
	float renderX = x - center.x;
	float renderY = y - center.y;
	if (activeSprite < sprites.size()) {
		if (sprites[activeSprite]->texture != NULL) {
			sprites[activeSprite]->render(renderX, renderY, NULL, direction, NULL);
		}
	}
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
// OBJECT FUNCTIONS (called by everyone)
// ////////////////////////////////////////////////////////////////
int Object::getCollisionLayer() {
	return collisionLayer;
}


bool Object::pointInsideBounds(float pointX, float pointY) {
	bool collisionDetected = false;
	float myLeft = x - center.x;
	float myRight = x + center.x;
	float myTop = y - center.y;
	float myBottom = y + center.y;

	if (pointX >= myLeft && pointX <= myRight && pointY >= myTop && pointY <= myBottom) {
		collisionDetected = true;
	}
	
	return collisionDetected;
}

// /////////////////////////////////
// SPRITE FUNCTIONS (called by child)
// ////////////////////////////////////////////////////////////////

void Object::addSprite(std::string path) {
	Sprite* s = new Sprite;
	if(!s->loadFromFile(path)) {
		printf("Could not load sprite\n");
		errorFound = 1;
	} else {
		//printf("doing sprite push back\n");
		//printf("--num sprites: %lu\n", sprites.size());
		sprites.push_back(s);
	}
	//printf("finished add sprite\n");
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