#pragma once

#include<list>
#include "Globals.h"
#include "Module.h"
#include "timer.h"
//#include <time.h>

class ModuleRender;
class ModuleWindow;
class ModuleTextures;
class ModuleInput;
class ModuleProgram;
class ModuleRenderExercise;
class ModuleEditor;
class ModuleCamera;
class ModuleDebugDraw;
class ModuleTexture;
class ModuleConsole;


//class Timer;

class Application
{
public:

	Application();
	~Application();

	bool Init();
	bool Start();
	update_status Update();
	bool CleanUp();

public:
	ModuleConsole* console = nullptr;
	ModuleRender* renderer = nullptr;
	ModuleWindow* window = nullptr;
	ModuleCamera* camera = nullptr;
	ModuleInput* input = nullptr;
	ModuleProgram* program = nullptr;
	ModuleTexture* textures = nullptr;
	ModuleRenderExercise * renderExercise = nullptr;
	ModuleEditor* editor = nullptr;
	ModuleDebugDraw* debug = nullptr;

	//clock_t t;
	Timer timer;
	float dt,realdt;

	int fps = 60;
private:

	std::list<Module*> modules;

};

extern Application* App;
