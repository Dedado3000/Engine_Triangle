#include "Globals.h"
#include "Application.h"
#include "ModuleRenderExercise.h"
#include "ModuleProgram.h"
#include "ModuleWindow.h"
#include "SDL.h"
#include "GL\glew.h"
#include <iostream>


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


	ourVbo = CreateTriangleVBO();

	return true;
}

update_status ModuleRenderExercise::PreUpdate()
{
	return UPDATE_CONTINUE;
}

// Called every draw update
update_status ModuleRenderExercise::Update()
{
	
	RenderVBO(ourVbo, App->program->program_id);
	return UPDATE_CONTINUE;
}

update_status ModuleRenderExercise::PostUpdate()
{

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderExercise::CleanUp()
{
	LOG("Destroying renderer example");


	DestroyVBO(ourVbo);

	return true;
}



//TriangleMethods
// This function must be called one time at creation of vertex buffer
unsigned ModuleRenderExercise::CreateTriangleVBO()
{
	float vtx_data[] = { -1.0f, -1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f };
	unsigned vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // set vbo active
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtx_data), vtx_data, GL_STATIC_DRAW);

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
	// size = 3 float per vertex
	// stride = 0 is equivalent to stride = sizeof(float)*3
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glUseProgram(program);
	// 1 triangle to draw = 3 vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);
}