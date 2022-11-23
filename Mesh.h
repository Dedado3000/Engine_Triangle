#pragma once
#include "Globals.h"
#include "assimp/scene.h"
#include <iostream>
#include <vector>

class Mesh
{
public:
	Mesh();
	~Mesh();

	void Load( aiMesh** mesh, unsigned int numMeshes);
	void LoadVBO(const aiMesh* mesh);
	void LoadEBO(const aiMesh* mesh);
	void CreateVAO();

	void Draw(const std::vector<unsigned>& model_textures);

	unsigned int vbo,ebo, vao;
	int num_index;
	int num_vertex;
};
