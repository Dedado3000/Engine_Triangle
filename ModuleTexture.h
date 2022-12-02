#pragma once
#include "Module.h"
#include "Globals.h"

#include "DirectXTex.h"
#include "GL\glew.h"
#include <string>

class ModuleTexture :
    public Module
{

public:
	ModuleTexture();
	~ModuleTexture();

	bool Init();

	GLuint LoadTexture(const char* image_file_name, const char* route_path, const char* route_fbx, std::string &size_data);
	GLuint FullLoadFromRoute(const char* rute_image_file_name, HRESULT& res, std::string& size_data);


private:

	float f = 0;
};

