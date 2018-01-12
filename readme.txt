=========================
SDL GAME LIBRARY
=========================

LIBRARY WORKFLOW:
	Class Sprite:
		sdl texture
		width
		height
		loadFromFile() -> texture
		render(x, y, rotation, ...) 
			// renders texture
			// parameters come from Object wrapper class

	Class Object:
		Sprite sprite
		x
		y
		xSpeed
		ySpeed
		direction
		update()
			// child overrides this
		render()
			// calls sprite.render()
		addSprite(path)
			// puts image into sprite
		keyUp()
			// child overrides this
		keyDown()
			// child overrides this

	FUNCTIONS
		sdl WINDOW
		sdl RENDERER
		sdl GAME_EVENT
			// keystrokes and such
		bool RUNNING
		vector Objects* objects
		INIT()
		CLOSE()
		GAME_UPDATE()
			if key = escape || close
				RUNNING = false
			for each object* in objects
				keyUp(GAME_EVENT)
				keyDown(GAME_EVENT)
				object->update()
				object->render()