#include "Model.h"
#include "assimp/cimport.h"
#include "assimp/postprocess.h"
#include "Application.h"
#include "ModuleTexture.h"
#include "Mesh.h"

using namespace std;

Model::Model()
{

}

Model::~Model()
{

}

void Model::Load(const char* file_name)
{
	
	const aiScene* scene = aiImportFile(file_name, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene)
	{
		LoadMaterials(scene);
		Mesh meshScene;
		meshScene.Load(scene->mMeshes, scene->mNumMeshes);
	}
	else
	{
		LOG_APP("Error loading %s: %s", file_name, aiGetErrorString());
	}
	
}


void Model::LoadMaterials(const aiScene* scene)
{
	aiString file;
	materials.reserve(scene->mNumMaterials);
	for (unsigned i = 0; i < scene->mNumMaterials; ++i)
	{
		if (scene->mMaterials[i]->GetTexture(aiTextureType_DIFFUSE, 0, &file) == AI_SUCCESS)
		{
			materials.push_back(App->textures->LoadTexture(file.data));
		}
	}
}

