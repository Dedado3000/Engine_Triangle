#pragma once

#include "Module.h"
#include "Globals.h"
#include <iostream>
#include <string>


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

	//Geometry Information
	std::string geometryLoaded;
	std::string textureLoaded;
	std::string triangleCount;
	std::string textureSize;

	float scale = 1.f;
	update_status continueUpdate = UPDATE_CONTINUE;
	//float ms_fps[25];
};
