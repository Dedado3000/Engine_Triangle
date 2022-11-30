#pragma once

#include "Module.h"
#include "Globals.h"


class ModuleEditor : public Module
{
public:
	ModuleEditor();
	~ModuleEditor();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	boolean editorWindows = true;

private:

	float scale = 1.f;
	update_status continueUpdate = UPDATE_CONTINUE;
	//float ms_fps[25];
};
