#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"

#include "SDL.h"
#include "SDL/include/SDL.h"

using namespace std;

ModuleCamera::ModuleCamera()
{
}

// Destructor
ModuleCamera::~ModuleCamera()
{
}

// Called before render is available
bool ModuleCamera::Init()
{
	/*
	*/

	return true;
}

bool ModuleCamera::Start()
{
	position = float3(0.0f, 4.0f, 10.0f);
	int w, h;
	fov = 90.0f;
	SDL_GetWindowSize(App->window->window, &w, &h);

	frustum.SetPerspective(math::pi / 4.0f, 2.f * atanf(tanf(math::pi / 4.0f * 0.5f) * (w / h)));
	//frustum.SetPos(float3::zero);
	frustum.SetPos(float3(0.0f, 4.0f, 20.0f));
	frustum.SetFront(-float3(0,0.2f,1.f));
	frustum.SetUp(float3::unitY);
	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetHorizontalFovAndAspectRatio(fov, 1.3f);

	//frustum.nearPlaneDistance = 0.1f;
	//frustum.farPlaneDistance = 100.0f;

	return true;

}

update_status ModuleCamera::PreUpdate()
{
	int x, y;
	Uint32 buttons;

	SDL_PumpEvents();  // make sure we have the latest mouse state.

	buttons = SDL_GetMouseState(&x, &y);

	const Uint8* keyboard = App->input->keyboard;

	float3 f = frustum.Front();
	float3 fLat = f.Perpendicular();

	float moveY = (clickedY - y);
	float moveX = (clickedX - x) ;
	//Drag Camera
	if (leftClick && middleClick && !rightClick)
	{

		float3 f = frustum.Front();
		position -= float3(-moveX * 0.01 * App->dt, moveY * 0.01 * App->dt, 0);

	}
	//Camera Move and Zoom and Rotate
	if(rightClick && !leftClick && !middleClick)
	{
		//frustum.SetFront(-float3(0, 0.2f, 1.f));

		if (keyboard[SDL_SCANCODE_Q])
		{
			position -= float3(0, 4*App->dt, 0);
		}
		if (keyboard[SDL_SCANCODE_E])
		{
			position -= float3(0, -4 * App->dt, 0);
		}
		if (keyboard[SDL_SCANCODE_W])
		{
			float3 f = frustum.Front();
			position -= float3(-f.x * App->dt * 4, -f.y * App->dt * 4, -f.z *App->dt * 4);
		}
		if (keyboard[SDL_SCANCODE_S])
		{
			position += float3(-f.x * App->dt * 4, -f.y * App->dt * 4, -f.z * App->dt * 4);
		}
		if (keyboard[SDL_SCANCODE_S])
		{
			position += float3(-f.x * App->dt * 4, -f.y * App->dt * 4, -f.z * App->dt * 4);
		}
		if (keyboard[SDL_SCANCODE_A])
		{
			position += float3(-fLat.x * App->dt * 4, -fLat.y * App->dt * 4, -fLat.z * App->dt * 4);
		}
		if (keyboard[SDL_SCANCODE_D])
		{
			position -= float3(-fLat.x * App->dt * 4, -fLat.y * App->dt * 4, -fLat.z * App->dt * 4);
		}

		//Zoom
		/*
		Temporal, must be in mouse scroll
		*/
		if (keyboard[SDL_SCANCODE_LALT])
		{
			float zoomY = (clickedY - y) * 3 * App->dt;
			fov = fov + zoomY;
			clickedY = y;
			if (fov < 80) 
				fov = 89;
			if (fov > 90.6) 
				fov = 90.6;
			frustum.SetHorizontalFovAndAspectRatio(fov , 1.3f);

		}
	}


	frustum.SetPos(position);
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleCamera::Update()
{

	return UPDATE_CONTINUE;
}

Frustum ModuleCamera::GetFrustum()
{
	return frustum;
}


// Called before quitting
bool ModuleCamera::CleanUp()
{
	return true;
}

