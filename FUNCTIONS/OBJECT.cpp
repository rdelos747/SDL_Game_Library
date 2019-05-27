// /////////////////////////////////
// O B J E C T
// ////////////////////////////////////////////////////////////////

#include "OBJECT.h"

// /////////////////////////////////
// CONSTRUCTOR
// ////////////////////////////////////////////////////////////////

Object::Object(/*Engine* newEgPtr*/) {

	// SET THE ENGINE
	//egPtr = newEgPtr;
	// OBJECT VARS
	destroy = false;
	ID = -1;
	collisionLayer = 0;
	x = 0;
	y = 0;
	direction = 0;
	center.x = 0;
	center.y = 0;

	// SPRITE VARS
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
	center.x = 0;
	center.y = 0;
	
	// sprites.clear();
	if (text != NULL) {
		text->freeText();
		text = NULL;
	}

	//egPtr = NULL;
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

std::string Object::getSprite() {
	return sprite;
}

void Object::setSprite(std::string key) {
	sprite = key;
}

void Object::showSprite() {
	visible = true;
}

void Object::hideSprite() {
	visible = false;
}

Size Object::getSpriteSize() {
	return ENGINE.getSpriteSize(sprite);
}

// /////////////////////////////////
// TEXT FUNCTIONS (called by child)
// ////////////////////////////////////////////////////////////////

void Object::linkFont(TTF_Font* font) {
	// should I clear the text object first if one exists???
	Text* t = new Text;
	t->font = font;
	if (t->font == NULL) {
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
    ENGINE.setError(1);
	} else {
		text = t;
	}
}

Text* Object::getText() {
	return text;
}

void Object::setText(std::string value, SDL_Color color) {
	if (text == NULL) {
		printf("Cannot set text value, object->text = NULL. Must first create text item with setTextFont()\n");
		ENGINE.setError(1);
	} else if (text->font == NULL) {
		printf("Cannot set text value, object->text->font = NULL. Must first create text item with setTextFont()\n");
		ENGINE.setError(1);
	} else {
		if (!text->loadFromRenderedText(ENGINE.renderer, value, color)) {
			printf( "Failed to render text texture!\n" );
			ENGINE.setError(1);
		}
	}
}

void Object::setText(int value, SDL_Color color) {
	if (text == NULL) {
		printf("Cannot set text value, object->text = NULL. Must first create text item with setTextFont()\n");
		ENGINE.setError(1);
	} else if (text->font == NULL) {
		printf("Cannot set text value, object->text->font = NULL. Must first create text item with setTextFont()\n");
		ENGINE.setError(1);
	} else {
		if (!text->loadFromRenderedText(ENGINE.renderer, std::to_string(value), color)) {
			printf( "Failed to render text texture!\n" );
			ENGINE.setError(1);
		}
	}
}
