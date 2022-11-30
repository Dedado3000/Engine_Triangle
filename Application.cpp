#pragma once
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRender.h"
#include "ModuleInput.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"
#include "ModuleEditor.h"
#include "ModuleRenderExercise.h"
#include "ModuleDebugDraw.h"
#include "ModuleTexture.h"
#include "ModuleConsole.h"

#include "Timer.h"

using namespace std;

Application::Application()
{

	// Order matters: they will Init/start/update in this order
	modules.push_back(console = new ModuleConsole());
	modules.push_back(input = new ModuleInput());
	modules.push_back(window = new ModuleWindow());
	modules.push_back(camera = new ModuleCamera());
	modules.push_back(editor = new ModuleEditor());
	modules.push_back(renderer = new ModuleRender());
	modules.push_back(program = new ModuleProgram());
	modules.push_back(textures = new ModuleTexture());
	modules.push_back(renderExercise = new ModuleRenderExercise());
	modules.push_back(debug = new ModuleDebugDraw());
	
	//Create Clock
	dt = realdt = 0.f;
	//t = clock();
}

Application::~Application()
{
	for(list<Module*>::iterator it = modules.begin(); it != modules.end(); ++it)
    {
        delete *it;
    }
}

bool Application::Init()
{
	bool ret = true;

	for(list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Init();

	return ret;
}
bool Application::Start()
{
	bool ret = true;

	for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret; ++it)
		ret = (*it)->Start();

	return ret;

	//Settting Timer
	//dt = clock() - t;
	//t = clock();
	timer.DeltaTime(dt, realdt);
}

update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	//Loop for FPS
	float fpsComprobator = timer.TicksSinceLastDT();
	if (fpsComprobator > 1000.f/ (float)fps)
	{
		//Update DT
		timer.DeltaTime(dt, realdt);
		//timer.GetFramesSinceStart();
		//float delta = clock() - t;


		for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
			ret = (*it)->PreUpdate();

		for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
			ret = (*it)->Update();

		for (list<Module*>::iterator it = modules.begin(); it != modules.end() && ret == UPDATE_CONTINUE; ++it)
			ret = (*it)->PostUpdate();

	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for(list<Module*>::reverse_iterator it = modules.rbegin(); it != modules.rend() && ret; ++it)
		ret = (*it)->CleanUp();

	return ret;
}
