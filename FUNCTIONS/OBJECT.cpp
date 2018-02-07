// /////////////////////////////////
// O B J E C T
// ////////////////////////////////////////////////////////////////

#include "OBJECT.h"

// /////////////////////////////////
// CONSTRUCTOR
// ////////////////////////////////////////////////////////////////

Object::Object() : sprites() {
	//printf(" --OBJECT CONSTRUCTOR %d\n", ID);

	// OBJECT VARS
	destroy = false;
	ID = currentID++;
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
	for (int i = 0; i < sprites.size(); i++) {
		delete sprites[i];
	}
	sprites.clear();
	if (text != NULL) {
		text->freeText();
		text = NULL;
	}
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
	if (visible == true) {
		float renderX = x - center.x;
		float renderY = y - center.y;
		if (activeSprite < sprites.size()) {
			if (sprites[activeSprite]->texture != NULL) {
				sprites[activeSprite]->render(renderX, renderY, NULL, direction, NULL);
			}
		}
		if (text != NULL) {
			if (text->texture != NULL & text->font != NULL) {
				text->render(x, y, NULL, direction, NULL);
			}
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
		sprites.push_back(s);
	}
}

void Object::setTextFont(std::string path, int size) {
	Text* t = new Text;
	t->font = TTF_OpenFont(path.c_str(), size);
	if (t->font == NULL) {
		printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        errorFound = 1;
	} else {
		text = t;
	}
}

void Object::setTextValue(std::string value, SDL_Color color) {
	if (text == NULL) {
		printf("Cannot set text value, object->text = NULL. Must first create text item with setTextFont()\n");
		errorFound = 1;
	} else if (text->font == NULL) {
		printf("Cannot set text value, object->text->font = NULL. Must first create text item with setTextFont()\n");
		errorFound = 1;
	} else {
		if (!text->loadFromRenderedText(value, color)) {
			printf( "Failed to render text texture!\n" );
			errorFound = 1;
		}
	}
}

void Object::setTextValue(int value, SDL_Color color) {
	if (text == NULL) {
		printf("Cannot set text value, object->text = NULL. Must first create text item with setTextFont()\n");
		errorFound = 1;
	} else if (text->font == NULL) {
		printf("Cannot set text value, object->text->font = NULL. Must first create text item with setTextFont()\n");
		errorFound = 1;
	} else {
		if (!text->loadFromRenderedText(std::to_string(value), color)) {
			printf( "Failed to render text texture!\n" );
			errorFound = 1;
		}
	}
}

void Object::showSprite() {
	visible = true;
}

void Object::hideSprite() {
	visible = false;
}

bool Object::isVisible() {
	return visible;
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