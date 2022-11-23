#pragma once

#include "Geometry/Frustum.h"
#include "Math/float3.h"

#include "Module.h"
#include "Globals.h"

class ModuleCamera : public Module
{
public:
	ModuleCamera();
	~ModuleCamera();

	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	bool CleanUp();
	Frustum GetFrustum();

	float3 position;
	bool leftClick=false;
	bool middleClick = false;
	bool rightClick = false;
	int clickedX, clickedY;
	float fov;
private:
	Frustum frustum;
};
