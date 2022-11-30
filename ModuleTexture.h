#pragma once
#include "Module.h"
#include "Globals.h"

#include "DirectXTex.h"
#include "GL\glew.h"

class ModuleTexture :
    public Module
{

public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();

	GLuint LoadTexture(const char* image_file_name, const char* route_path, const char* route_fbx);
	GLuint FullLoadFromRoute(const char* rute_image_file_name, HRESULT& res);


private:

	float f = 0;
};

