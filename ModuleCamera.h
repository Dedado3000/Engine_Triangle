#pragma once

#include "Geometry/Frustum.h"
#include "Math/float3.h"
#include "Math/float3x3.h"

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

	void FocusPoint();
	void GetRotationMovement(float x, float z, int& valueX, int& valueZ, int motion);
	void Resize();
	void Relocate();

	float3 position;
	bool leftClick = false;
	bool middleClick = false;
	bool rightClick = false;
	int clickedX, clickedY;
	int mouseWheel = 0;
	int movementSpeed = 1;
	int rotationSpeed = 1;
	float fov;
private:
	Frustum frustum;
};
