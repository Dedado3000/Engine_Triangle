
#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRender.h"
#include "imgui_impl_sdl.h"
#include "SDL/include/SDL.h"

ModuleInput::ModuleInput()
{}

// Destructor
ModuleInput::~ModuleInput()
{}

// Called before render is available
bool ModuleInput::Init()
{
    LOG_APP("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
        LOG_APP("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate()
{
    SDL_Event sdlEvent;

    while (SDL_PollEvent(&sdlEvent) != 0)
    {
        //Aqui va el evento de raton
        switch (sdlEvent.type)
        {
            case SDLK_1:
            case SDL_QUIT:
                return UPDATE_STOP;
            case SDL_WINDOWEVENT:
                if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED || sdlEvent.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    App->renderer->WindowResized(sdlEvent.window.data1, sdlEvent.window.data2);
                break;
            case SDL_MOUSEBUTTONDOWN:

                SDL_GetMouseState(&App->camera->clickedX, &App->camera->clickedY);
                
                if(sdlEvent.button.button == SDL_BUTTON_RIGHT)
                    App->camera->rightClick = true;
                else if (sdlEvent.button.button == SDL_BUTTON_LEFT)
                    App->camera->leftClick = true;
                else if (sdlEvent.button.button == SDL_BUTTON_MIDDLE)
                    App->camera->middleClick = true;
                break;
            case SDL_MOUSEBUTTONUP:
                if (sdlEvent.button.button == SDL_BUTTON_RIGHT)
                    App->camera->rightClick = false;
                else if (sdlEvent.button.button == SDL_BUTTON_LEFT)
                    App->camera->leftClick = false;
                else if (sdlEvent.button.button == SDL_BUTTON_MIDDLE)
                    App->camera->middleClick = false;
                break;
            case SDL_MOUSEWHEEL:
                App->camera->mouseWheel = - sdlEvent.wheel.y;
                break;
        }
    }
    ImGui_ImplSDL2_ProcessEvent(&sdlEvent);
    keyboard = SDL_GetKeyboardState(NULL);

    return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
    LOG_APP("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}
