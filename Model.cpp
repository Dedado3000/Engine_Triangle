
#include "Model.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleConsole.h"


#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include <string>

using namespace std;

Model::Model()
{

}

Model::~Model()
{

}

void Model::Load(const char* file_name)
{
	
	scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		LoadMaterials(scene, file_name);
		meshScene.Load(scene->mMeshes, scene->mNumMeshes);
	}
	else
	{
		LOG_APP("Error loading %s: %s", file_name, aiGetErrorString());
		App->console->AddToConsole("Error loading:");
		App->console->AddToConsole(aiGetErrorString());
	}

	totalTriangles = 0;
	for (int v : meshScene.num_indexV)
	{
		totalTriangles += v / 3;
	}
	file = file_name;
	//App->editor->ChangedGeometry(file_name, texture_name, totalTriangles+"", "");

}

void Model::ReLoad(const char* file_name)
{
	materials.clear();
	Load(file_name);
	/*scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		LoadMaterials(scene, file_name);
		meshScene.Load(scene->mMeshes, scene->mNumMeshes);
	}
	else
	{
		LOG_APP("Error loading %s: %s", file_name, aiGetErrorString());
		App->console->AddToConsole("Error loading:");
		App->console->AddToConsole(aiGetErrorString());
	}*/

}

void Model::LoadMaterials(const aiScene* scene, const char* file_name)
{
	aiString file;
	materials.reserve(scene->mNumMaterials);


	//Calculate the route from drop
	string route = file_name;
	const char* fbx;
	std::size_t pos = 0;
	char* route_fbx;
	if (route.find("//") != string::npos )
	{
		pos = route.find("//");
	}
	else if (route.find("\\") != string::npos)
	{
		pos = route.find_last_of("\\");
	}
	if (pos != 0)
	{
		route_fbx = new char[pos+1];
		memmove(route_fbx, file_name ,  pos+1);
		route_fbx[pos+1] = 0;
		for (int i = 0; i < pos + 1; i++) {
			char character = route_fbx[i];
			if (route_fbx[i] == '\\')
				route_fbx[i] = '//';
		}
		fbx =  route_fbx;

	}
	else {
		fbx = "";
	}

	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			texture_name = file.data;
			materials.push_back(App->textures->LoadTexture(file.data, "", fbx,texturesSize));
		}
	}
}

void Model::Draw()
{
	meshScene.Draw(materials);
}


void Model::GetGeometry(std::string &geometry, std::string &texture, std::string &triangles, std::string &textureSize) {
	geometry = file;
	texture = texture_name;
	triangles = to_string(totalTriangles);
	textureSize = texturesSize+"";
}