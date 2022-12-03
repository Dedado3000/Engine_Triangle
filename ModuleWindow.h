#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow();

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	update_status PreUpdate();

	// Called before quitting
	bool CleanUp();

public:
	//The window we'll be rendering to
	SDL_Window* window = NULL;

	//The surface contained by the window
	SDL_Surface* screen_surface = NULL;

	int height, width, lastHeight, lastWidth;
	int maxHeight, maxWidth;
};

#endif // __ModuleWindow_H__