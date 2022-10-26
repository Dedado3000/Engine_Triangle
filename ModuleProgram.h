#pragma once
#include "Module.h"
#include "Globals.h"

typedef unsigned __int8 Uint8;

class ModuleProgram : public Module
{
public:

	ModuleProgram();
	~ModuleProgram();

	bool Init();
	update_status Update();
	bool CleanUp();

	char* LoadShaderSource(const char* shader_file_name);
	unsigned CompileShader(unsigned type, const char* source);
	unsigned CreateProgram(unsigned vtx_shader, unsigned frg_shader);

	unsigned program_id;
};