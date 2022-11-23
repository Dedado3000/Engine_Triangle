#pragma once
#include "Globals.h"
#include "assimp/scene.h"
#include <iostream>
#include <vector>

class Model
{
public:
	Model();
	~Model();

	void Load(const char* file_name);
	void LoadMaterials(const aiScene* scene);

	std::vector<GLuint> materials;
};

