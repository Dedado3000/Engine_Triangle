#include "Mesh.h"

#include "Application.h"
#include "ModuleTexture.h"
#include "ModuleProgram.h"
#include "ModuleCamera.h"

#include "assimp/mesh.h"
//#include "assimp/postprocess.h"

#include "Math/float2.h"

using namespace std;

Mesh::Mesh()
{

}

Mesh::~Mesh()
{

}

void Mesh::Load(aiMesh** mesh, unsigned int numMeshes)
{
	/**/
	vertexValuesX.clear();
	vertexValuesY.clear();
	vertexValuesZ.clear();
	for (int i = 0; i < numMeshes; i++)
	{
		LoadVBO(mesh[i]);
		LoadEBO(mesh[i]);
		CreateVAO();
	}
	/** /
	LoadVBO(mesh[0]);
	LoadEBO(mesh[0]);
	CreateVAO();
	/**/
}

void Mesh::LoadVBO(const aiMesh* mesh)
{
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	unsigned vertex_size = (sizeof(float) * 3 + sizeof(float) * 2);
	unsigned buffer_size = vertex_size * mesh->mNumVertices;
	glBufferData(GL_ARRAY_BUFFER, buffer_size, nullptr, GL_STATIC_DRAW);
	unsigned position_size = sizeof(float) * 3 * mesh->mNumVertices;
	glBufferSubData(GL_ARRAY_BUFFER, 0, position_size, mesh->mVertices);
	unsigned uv_offset = position_size;
	unsigned uv_size = sizeof(float) * 2 * mesh->mNumVertices;
	float2* uvs = (float2*)(glMapBufferRange(GL_ARRAY_BUFFER, uv_offset, uv_size, GL_MAP_WRITE_BIT));
	for (unsigned i = 0; i < mesh->mNumVertices; ++i)
	{
		uvs[i] = float2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	num_vertex = mesh->mNumVertices;
	for (int i = 0; i < sizeof( mesh->mVertices ) ; i++) {
		vertexValuesX.push_back(mesh->mVertices[i].x);
		vertexValuesY.push_back(mesh->mVertices[i].y);
		vertexValuesZ.push_back(mesh->mVertices[i].z);
	}
}

void Mesh::LoadEBO(const aiMesh* mesh)
{
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	unsigned index_size = sizeof(unsigned) * mesh->mNumFaces * 3;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_size, nullptr, GL_STATIC_DRAW);
	unsigned* indices = (unsigned*)(glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY)); //GL_MAP_WRITE_BIT / GL_WRITE_ONLY
	for (unsigned i = 0; i < mesh->mNumFaces; ++i)
	{
		assert(mesh->mFaces[i].mNumIndices == 3); // note: assume triangles = 3 indices per face
		*(indices++) = mesh->mFaces[i].mIndices[0];
		*(indices++) = mesh->mFaces[i].mIndices[1];
		*(indices++) = mesh->mFaces[i].mIndices[2];
	}
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	num_index = mesh->mNumFaces * 3;
}

void Mesh::CreateVAO()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)(sizeof(float) * 3 * num_vertex));
}


void Mesh::Draw(const std::vector<unsigned>& model_textures)
{
	unsigned program = App->program->program_id;

	Frustum frust = App->camera->GetFrustum();

	const float4x4 proj = frust.ProjectionMatrix();
	const float4x4 view = frust.ViewMatrix();

	//const float4x4& view = App->camera->GetView();
	//const float4x4& proj = App->camera->GetProjection();
	float4x4 model = float4x4::identity;
	glUseProgram(program);
	glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_TRUE, (const float*)&model);
	glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_TRUE, (const float*)&view);
	glUniformMatrix4fv(glGetUniformLocation(program, "proj"), 1, GL_TRUE, (const float*)&proj);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model_textures[0]); // glBindTexture(GL_TEXTURE_2D, model_textures[material_index]);
	glUniform1i(glGetUniformLocation(program, "diffuse"), 0);
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, num_index, GL_UNSIGNED_INT, nullptr);
}

void Mesh::MiddlePoint(float& xValue, float& yValue, float& zValue)
{
	xValue = 0;
	yValue = 0;
	zValue = 0;
	for (float value : vertexValuesX)
	{
		xValue += value;
	}
	for (float value : vertexValuesY)
	{
		yValue += value;
	}
	for (float value : vertexValuesZ)
	{
		zValue += value;
	}
	xValue = xValue / vertexValuesX.size();
	yValue = yValue / vertexValuesY.size();
	zValue = zValue / vertexValuesZ.size();
}


void Mesh::FarAwayPoint(float& xValue, float& yValue, float& zValue) {
	for (float value : vertexValuesX)
	{
		if(value>xValue)
			xValue = value;
	}
	for (float value : vertexValuesY)
	{
		if (value > yValue)
			yValue = value;
	}
	for (float value : vertexValuesZ)
	{
		if (value > zValue)
			zValue = value;
	}
}