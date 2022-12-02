#include "ModuleCamera.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderExercise.h"


#include "Model.h"
#include "Mesh.h"

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
	position = float3(8.0f, 2.0f, 8.0f);
	int w, h;
	fov = 90.0f;
	SDL_GetWindowSize(App->window->window, &w, &h);

	frustum.SetPerspective(math::pi / 4.0f, 2.f * atanf(tanf(math::pi / 4.0f * 0.5f) * (w / h)));
	//frustum.SetPos(float3::zero);
	frustum.SetPos(position);
	frustum.SetFront(-float3(0.5f,0.f,0.5f));
	frustum.SetUp(float3::unitY);
	frustum.SetViewPlaneDistances(0.1f, 300.0f);
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetHorizontalFovAndAspectRatio(fov, (w / h));

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

	//e. Holding SHIFT duplicates movement speed.
	float speed = 1;
	if (keyboard[SDL_SCANCODE_LSHIFT] || keyboard[SDL_SCANCODE_RSHIFT])
	{
		speed = 2;
	}

	//Camera Move and Rotate
	//a. While Right clicking, “WASD” fps-like movement and free look around must be enabled
	if(rightClick && !leftClick && !middleClick)
	{
		//frustum.SetFront(-float3(0, 0.2f, 1.f));

		if (keyboard[SDL_SCANCODE_Q])
		{
			position -= float3(0, 4 * App->dt * speed, 0);
		}
		if (keyboard[SDL_SCANCODE_E])
		{
			position -= float3(0, -4 * App->dt * speed, 0);
		}
		if (keyboard[SDL_SCANCODE_W])
		{
			position -= float3(-f.x * App->dt * 4 * speed, -f.y * App->dt * 4 * speed, -f.z * App->dt * 4 * speed);
		}
		if (keyboard[SDL_SCANCODE_S])
		{
			position += float3(-f.x * App->dt * 4 * speed, -f.y * App->dt * 4 * speed, -f.z * App->dt * 4 * speed);
		}
		if (keyboard[SDL_SCANCODE_A])
		{
			position += float3(-fLat.x * App->dt * 4 * speed, -fLat.y * App->dt * 4 * speed, -fLat.z * App->dt * 4 * speed);
		}
		if (keyboard[SDL_SCANCODE_D])
		{
			position -= float3(-fLat.x * App->dt * 4 * speed, -fLat.y * App->dt * 4 * speed, -fLat.z * App->dt * 4 * speed);
		}
		frustum.SetPos(position);

		// Free Look
		//2d plane moving
		//Acording with the look, the xyz space is compeled with 2 directions in a 2D plane and a direction for the 3rd, 
		//we will use the move of the mouse in Y for the 3rd direction and use the X of the mouse for left and right direction
		if (!keyboard[SDL_SCANCODE_F]) {

			float rotationSpeed = 0.006;
			float3x3 rotationY = float3x3::RotateY( App->dt * speed * rotationSpeed * moveX);
			frustum.SetWorldMatrix(rotationY * frustum.WorldMatrix());
			float3x3 rotationAxis = float3x3::RotateAxisAngle(frustum.WorldRight(), App->dt * speed * rotationSpeed * moveY);
			frustum.SetWorldMatrix(rotationAxis * frustum.WorldMatrix());

		}

		/*
		//Zoom
		//Deprecated from this method to mousewheel
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
		*/
	}

	//b. Mouse wheel should zoom in and out.
	if (mouseWheel!=0)
	{
		float zoomY = mouseWheel * speed * 3 * App->dt;
		fov = fov + zoomY;
		clickedY = y;
		if (fov < 88)
			fov = 88;
		if (fov > 90.6)
			fov = 90.6;
		frustum.SetHorizontalFovAndAspectRatio(fov, 1.3f);
		mouseWheel = 0;
	}
	// d. Pressing “f” should focus the camera around the geometry.
	if (keyboard[SDL_SCANCODE_F]) {
		FocusPoint();

	}
	//c. Alt+Left click should orbit the object.
	if (keyboard[SDL_SCANCODE_LALT] && leftClick && !rightClick)
	{
		FocusPoint();
		float xVal = 0;
		float yVal = 0;
		float zVal = 0;
		App->renderExercise->chargedModel.meshScene.MiddlePoint(xVal, zVal, yVal);
		float distance1 =sqrt(  pow(position.x - xVal,2) + pow(position.y - yVal,2) + pow(position.z - zVal,2) );

		//position += float3(f.x * App->dt * 4 * speed * moveX, f.y * App->dt * 4 * speed * moveX, f.z * App->dt * 4 * speed * moveX);
		float baseRot = 0.015f;
		//Moving in XZ 
		float3 rotationXZ{ -fLat.x * App->dt * speed * moveX * baseRot, /*-fLat.y * App->dt * speed * moveX * baseRot */ 0, -fLat.z * App->dt * speed * moveX * baseRot };
		position += rotationXZ;
		float3 rotationY{ 0, App->dt * speed * moveY * baseRot , 0 };
		position += rotationY;

		//Move the camera forward
		//Calculate the diference of distance Focus->P1 to distance Focus->P2
		frustum.SetPos(position);
		f = frustum.Front();
		FocusPoint();
		float distance2 = sqrt(pow(position.x - xVal, 2) + pow(position.y - yVal, 2) + pow(position.z - zVal, 2));
		float calculatedMovement = distance2 - distance1;
		position -= float3(-f.x * calculatedMovement, -f.y * calculatedMovement, -f.z *  calculatedMovement);
	}

	if (keyboard[SDL_SCANCODE_I]) {
		frustum.SetFront(-float3(0.5f, 0.f, 0.5f));

	}
	if (keyboard[SDL_SCANCODE_O]) {
		frustum.SetFront(-float3(0.5f, 0.5f, 0.f));

	}

	frustum.SetPos(position);
	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);
	frustum.SetHorizontalFovAndAspectRatio(fov, (float)w/(float)h);
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


void ModuleCamera::FocusPoint()
{

	float xVal = 0;
	float yVal = 0;
	float zVal = 0;
	App->renderExercise->chargedModel.meshScene.MiddlePoint(xVal, yVal, zVal);
	float3 totaldirectionVector = float3{ xVal, yVal, zVal } -frustum.Pos();
	float3 directionVector = totaldirectionVector.Normalized();
	float3 up = frustum.WorldRight().Cross(directionVector).Normalized();
	float3x4 look = frustum.WorldMatrix().LookAt(float3::unitZ, frustum.Pos() -float3{ xVal, yVal, zVal } , float3::unitY, float3::unitY);
	frustum.SetWorldMatrix(look );
}

void ModuleCamera::GetRotationMovement(float x, float z, int& valueX, int& valueZ, int motion)
{
	if (x >= 0 && z >= 0)
	{
		valueX = 1;
		valueZ = -1;
	}
	else if (x >= 0 && z <= 0)
	{
		valueX = -1;
		valueZ = -1;
	}
	else if (x <= 0 && z >= 0)
	{
		valueX = 1;
		valueZ = 1;
	}
	else if (x <= 0 && z <= 0)
	{
		valueX = -1;
		valueZ = 1;
	}
}

void ModuleCamera::Resize() {

	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);

	frustum.SetPerspective(math::pi / 4.0f, 2.f * atanf(tanf(math::pi / 4.0f * 0.5f) * (w / h)));


	frustum.SetViewPlaneDistances(0.1f, 100.0f);
	frustum.SetKind(FrustumSpaceGL, FrustumRightHanded);
	frustum.SetHorizontalFovAndAspectRatio(fov, (w / h));
}


void ModuleCamera::Relocate() {

	float xVal = 0;
	float yVal = 0;
	float zVal = 0;
	App->renderExercise->chargedModel.meshScene.FarAwayPoint(xVal, yVal, zVal);
	position = float3{ xVal * 2, yVal, zVal * 2 };
	frustum.SetPos(position);
}