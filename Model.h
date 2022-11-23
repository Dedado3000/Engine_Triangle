#pragma once
#include "Globals.h"
#include <iostream>
#include <vector>

#include "Mesh.h"


class Model
{
public:
	Model();
	~Model();

	void Load(const char* file_name);
	void LoadMaterials(const aiScene* scene);

	void Draw();

	std::vector<unsigned> materials;
	Mesh meshScene;
	const aiScene* scene;

};

