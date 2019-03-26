// /////////////////////////////////
// O B J E C T
// ////////////////////////////////////////////////////////////////

#include "OBJECT.h"

// /////////////////////////////////
// CONSTRUCTOR
// ////////////////////////////////////////////////////////////////

Object::Object(Engine* newEgPtr) {
	//printf(" --OBJECT CONSTRUCTOR %d\n", ID);

	// SET THE ENGINE
	egPtr = newEgPtr;
	// OBJECT VARS
	destroy = false;
	//ID = currentID++; // this should not be set here...
	// when user calls engine.addObject, the pushed object is assigned an id there
	ID = -1;
	collisionLayer = 0;
	x = 0;
	y = 0;
	direction = 0;
	center.x = 0;
	center.y = 0;

	// SPRITE VARS
	activeSprite = 0;
	text = NULL;
	visible = true;
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
	
	sprites.clear();
	if (text != NULL) {
		text->freeText();
		text = NULL;
	}

	egPtr = NULL;
}

// /////////////////////////////////
// VIRTUAL OVERRIDES
// ////////////////////////////////////////////////////////////////

void Object::update() {}
void Object::keyDown(int k) {}
void Object::keyUp(int k) {}

// /////////////////////////////////
// DESTROY (called by child)
// ////////////////////////////////////////////////////////////////

void Object::selfDestroy() { 
	destroy = true; //find a way to do this without just setting a flag.. maybe call Engine.destroy(self) or something...
}

bool Object::isDestroyed() {
	return destroy;
}

// /////////////////////////////////
// RENDER (called by engine)
// ////////////////////////////////////////////////////////////////
void Object::render() {
	if (visible == true) {
		//int index = objects[i]->getActiveSprite();
		float renderX = x - center.x;
		float renderY = y - center.y;

		if (activeSprite >= 0 && activeSprite < sprites.size()) {
			if (sprites[activeSprite]->texture != NULL) {
				sprites[activeSprite]->render(egPtr->renderer, renderX - egPtr->camera.x, renderY - egPtr->camera.y, NULL, direction, NULL);
			}
		}
		if (text != NULL) {
			if (text->texture != NULL & text->font != NULL) {
				text->render(egPtr->renderer, x - egPtr->camera.x, y - egPtr->camera.y, NULL, direction, NULL);
			}
		}
		// if (text != NULL) {
		// 	if (text->texture != NULL & text->font != NULL) {
		// 		text->render(x - CAMERA.x, y - CAMERA.y, NULL, direction, NULL);
		// 	}
		// }
	}
}

// /////////////////////////////////
// OBJECT FUNCTIONS (called by everyone)
// ////////////////////////////////////////////////////////////////
int Object::getID() {
	return ID;
}

int Object::setID(int newID) {
	if (ID == -1) {
		ID = newID;
	}
	else {
		printf("Object ID already set as %d\n", ID);
	}

	return ID;
}

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

void Object::linkSprite(Sprite* newSprite) {
	// sprites are passed around by their index within the engine.
	sprites.push_back(newSprite);
}

void Object::linkFont(TTF_Font* font) {
	// should I clear the text object first if one exists???
	Text* t = new Text;
	t->font = font;
	if (t->font == NULL) {
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    egPtr->setError(1);
	} else {
		text = t;
	}
}

void Object::setText(std::string value, SDL_Color color) {
	if (text == NULL) {
		printf("Cannot set text value, object->text = NULL. Must first create text item with setTextFont()\n");
		egPtr->setError(1);
	} else if (text->font == NULL) {
		printf("Cannot set text value, object->text->font = NULL. Must first create text item with setTextFont()\n");
		egPtr->setError(1);
	} else {
		if (!text->loadFromRenderedText(egPtr->renderer, value, color)) {
			printf( "Failed to render text texture!\n" );
			egPtr->setError(1);
		}
	}
}

void Object::setText(int value, SDL_Color color) {
	if (text == NULL) {
		printf("Cannot set text value, object->text = NULL. Must first create text item with setTextFont()\n");
		egPtr->setError(1);
	} else if (text->font == NULL) {
		printf("Cannot set text value, object->text->font = NULL. Must first create text item with setTextFont()\n");
		egPtr->setError(1);
	} else {
		if (!text->loadFromRenderedText(egPtr->renderer, std::to_string(value), color)) {
			printf( "Failed to render text texture!\n" );
			egPtr->setError(1);
		}
	}
}

void Object::showSprite() {
	visible = true;
}

void Object::hideSprite() {
	visible = false;
}

// bool Object::isVisible() {
// 	return visible;
// }

void Object::nextSprite() {
	activeSprite++;
	if (activeSprite >= sprites.size()) {
		activeSprite = 0;
	}
}

void Object::prevSprite() {
	if (sprites.size() == 0) {
		activeSprite = 0;
	}
	else {
		activeSprite--;
		if (activeSprite < 0) {
			activeSprite = sprites.size() - 1;
		}
	}
}

// int Object::getActiveSprite() {
// 	return sprites[activeSprite];
// }

//move these to engine since the engine is the only thing that has the sprites.
// int Object::getActiveSpriteWidth() {
// 	return sprites[activeSprite]->getWidth();
// }

// int Object::getActiveSpriteHeight() {
// 	return sprites[activeSprite]->getHeight();
// }