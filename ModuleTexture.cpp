#include "ModuleTexture.h"
#include "Application.h"
#include "ModuleConsole.h"

#include <string>
using namespace DirectX;
using namespace std;

ModuleTexture::ModuleTexture()
{}

// Destructor
ModuleTexture::~ModuleTexture()
{}

// Called before render is available
bool ModuleTexture::Init()
{
	bool ret = true;

	return ret;
}

GLuint ModuleTexture::LoadTexture(const char* image_file_name, const char* route_path, const char* route_fbx, std::string &size_data) {
	GLuint image;
	HRESULT res;
	const int size_image_file_name = strlen(image_file_name);
	//First Load full 
	image = FullLoadFromRoute(image_file_name, res, size_data);

	App->console->AddToConsole("------CHARGING NEW TEXTURE------");
	if (FAILED(res)) {
		const char* log = "Not Found at the route from Path Route "; //+ route_path + "/" + image_file_name ;
		App->console->AddToConsole(log);

		//route_fbx + route_path
		char* fbx = new char[strlen(image_file_name) + strlen(route_fbx) + 1 + 1]; // + 1 char + 1 for null;
		strcpy(fbx, route_fbx);
		strcat(fbx, image_file_name);

		image = FullLoadFromRoute(fbx, res, size_data);
		if (FAILED(res)) {
			const char* log = "Not Found at the route from PBX"; //+ route_fbx + "/" + image_file_name ;
			App->console->AddToConsole(log);


			char* ret = new char[strlen(image_file_name) + 10 + 1 + 1]; // + 1 char + 1 for null;
			strcpy(ret, "Textures\\");
			strcat(ret, image_file_name);
			image = FullLoadFromRoute(ret, res, size_data);
			if (!FAILED(res))
			{
				App->console->AddToConsole("Loaded Texture from Texture Carpet");
			}
			else {
				const char* log = "Not Found at the route from Texture Carpet";
			}
		}
		else {
			App->console->AddToConsole("Loaded Texture from FBX Path");
		}
	}
	else {
		App->console->AddToConsole("Loaded Texture from Path Route");
	}
	
	if (FAILED(res))
	{
		App->console->AddToConsole("Texture not found");
	}
	

	return image;
}


GLuint ModuleTexture::FullLoadFromRoute(const char* rute_image_file_name, HRESULT &res, std::string& size_data) {
	GLuint textureID=0;
	ScratchImage sImage;
	wchar_t* w_image = new wchar_t[strlen(rute_image_file_name) + 1];
	mbstowcs_s(NULL, w_image, strlen(rute_image_file_name) + 1, rute_image_file_name, strlen(rute_image_file_name));


	res = LoadFromDDSFile(w_image, DDS_FLAGS_NONE, nullptr, sImage);
	
	if (FAILED(res))
	{
		res = LoadFromTGAFile(w_image, TGA_FLAGS_NONE, nullptr, sImage);

		if (FAILED(res))
		{
			res = LoadFromWICFile(w_image, WIC_FLAGS_NONE, nullptr, sImage);
		}
	}


	if (!FAILED(res))
	{
		ScratchImage flipImage;
		FlipRotate(sImage.GetImages(), sImage.GetImageCount(), sImage.GetMetadata(), TEX_FR_FLIP_VERTICAL, flipImage);
		const Image* image = flipImage.GetImage(0, 0, 0);
		size_data = to_string(image->width) + " x " + to_string(image->height);
		//Create and Bind texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Add Some Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		GLint internalFormat, format, type;
		//Extract Metadata
		switch (flipImage.GetMetadata().format)
		{
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			internalFormat = GL_RGBA8;
			format = GL_RGBA;
			type = GL_UNSIGNED_BYTE;
			break;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			internalFormat = GL_RGBA8;
			format = GL_BGRA;
			type = GL_UNSIGNED_BYTE;
			break;
		case DXGI_FORMAT_B5G6R5_UNORM:
			internalFormat = GL_RGB8;
			format = GL_BGR;
			type = GL_UNSIGNED_BYTE;
			break;
		default:
			assert(false && "Unsupported format");
		}

		//Load Image
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, image->width, image->height, 0, format, type, image->pixels);
		//Add Some MipMap
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	return textureID;
}