

#include "assimp/include/assimp/mesh.h"
#include "MathGeoLib.h"
#include "ModuleCamera.h"

#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleRenderExercise.h"
#include "ModuleDebugDraw.h"
#include "ModuleTexture.h"

#include "SDL.h"


using namespace DirectX;
using namespace std;

ModuleRenderExercise::ModuleRenderExercise()
{
}

// Destructor
ModuleRenderExercise::~ModuleRenderExercise()
{
}

// Called before render is available
bool ModuleRenderExercise::Init()
{
	chargedModel.Load("BakerHouse.fbx");

	//ourVbo = CreateTriangleVBO();

	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRenderExercise::Update()
{
	
	//RenderVBO(ourVbo, App->program->program_id);
	RenderTriangle(ourVbo, App->program->program_id);
	chargedModel.Draw();
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercise::CleanUp()
{
	LOG_APP("Destroying renderer example");


	DestroyVBO(ourVbo);

	glDeleteTextures(1,&textureID);

	return true;
}



//TriangleMethods
// This function must be called one time at creation of vertex buffer
unsigned ModuleRenderExercise::CreateTriangleVBO()
{
	float vtx_data[] = 
				  {
					0.0f, 0.0f, 0.0f, //<- V2 pos
					1.0f, 0.0f, 0.0f, //<- V0 pos
					1.0f, 1.0f, 0.0f, //<- V1 pos
					0.0f, 0.0f, 0.0f, //<- V2 pos
					1.0f, 1.0f, 0.0f, //<- V1 pos
					0.0f, 1.0f, 0.0f, //<- V3 pos

					0.0f, 0.0f, //v2 textCoords
					1.0f, 0.0f, //v0 textCoords
					1.0f, 1.0f, //v1 textCoords
					0.0f, 0.0f, //v2 textCoords
					1.0f, 1.0f, //v1 textCoords
					0.0f, 1.0f, //v3 textCoords
	
	};

	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

	textureID = App->textures->LoadTexture("Test-image-Baboon.ppm", "", "");

	/**/


	/**/

	return vbo;
}
// This function must be called one time at destruction of vertex buffer
void ModuleRenderExercise::DestroyVBO(unsigned vbo)
{
	glDeleteBuffers(1, &vbo);
}

// This function must be called each frame for drawing the triangle
void ModuleRenderExercise::RenderVBO(unsigned vbo, unsigned program)
{
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glUseProgram(program);

	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void ModuleRenderExercise::RenderTriangle(unsigned vbo, unsigned program)
{

	int w, h;
	SDL_GetWindowSize(App->window->window, &w, &h);

	
	float4x4 model = float4x4::FromTRS(float3(0.0f, 0.0f, 0.0f),
		/*float4x4::RotateZ(pi / 4.0f)*/ float4x4::RotateZ(0),
		float3(1.0f, 1.0f, 0.0f));

	Frustum frust = App->camera->GetFrustum();

	float4x4 proj = frust.ProjectionMatrix();
	float4x4 view = frust.ViewMatrix();
	/*
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);



	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0,
		(void*)(sizeof(float) * 3 * 6) // buffer offset
	);

	glUseProgram(program);
	//Bind and use texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(program, "mytexture"), 0);


	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, &model[0][0]);
	//glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &App->camera->view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, &view[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(program,"proj"), 1, GL_TRUE, &proj[0][0]);


	// TODO: bind buffer and vertex attributes
	glDrawArrays(GL_TRIANGLES, 0, 6);
	*/

	//Calliong draw Module Debug

	App->debug->Draw(view, proj, w, h);
}