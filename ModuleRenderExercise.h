#pragma once
#include "Module.h"
#include "ModuleProgram.h"
#include "Globals.h"

#include "DirectXTex.h"
#include "GL\glew.h"

struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

class ModuleRenderExercise : public Module
{
public:
	ModuleRenderExercise();
	~ModuleRenderExercise();

	bool Init();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	//Triangle
	unsigned CreateTriangleVBO();
	void DestroyVBO(unsigned vbo);
	void RenderVBO(unsigned vbo, unsigned program);
	void RenderTriangle(unsigned vbo, unsigned program);

private:
	unsigned int ourVbo;
	GLuint textureID;
};