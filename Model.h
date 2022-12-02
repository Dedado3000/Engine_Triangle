#pragma once
#include "Globals.h"
#include <iostream>
#include <vector>
#include <string>

#include "Mesh.h"


class Model
{
public:
	Model();
	~Model();

	void Load(const char* file_name);
	void ReLoad(const char* file_name);
	void LoadMaterials(const aiScene* scene, const char* file_name);

	void Draw();
	void GetGeometry(std::string &geometry, std::string &texture, std::string &triangles, std::string &textureSize);

	std::vector<unsigned> materials;
	Mesh meshScene;
	const aiScene* scene;

private:
	const char* file="";
	std::string texture_name="";
	int totalTriangles = 0;
	std::string texturesSize="";

};

