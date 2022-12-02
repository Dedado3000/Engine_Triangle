#pragma once
#include "Globals.h"
#include "assimp/scene.h"
#include <iostream>
#include <vector>
#include "ModuleCamera.h"

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
	void MiddlePoint(float &xValue, float &yValue, float &zValue);
	void FarAwayPoint(float& xValue, float& yValue, float& zValue);

	std::vector<unsigned int> vboV,eboV, vaoV;
	std::vector<int> num_indexV;
	std::vector<int> num_vertexV;

	unsigned int vbo,ebo,vao;
	int num_index;
	int num_vertex;
	float3 position,scale;
	std::vector < float > vertexValuesX;
	std::vector < float > vertexValuesY;
	std::vector < float > vertexValuesZ;
	
};
