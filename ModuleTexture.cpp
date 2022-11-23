#include "ModuleTexture.h"


using namespace DirectX;

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

GLuint ModuleTexture::LoadTexture(const char* image_file_name) {
	GLuint image;

	//First Load full 
	image = FullLoadFromRoute(image_file_name);

	//Charging texture
	/*
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture_object);
	glUniform1i(glGetUniformLocation(program, "mytexture"), 0);
	namespace directX
	*/

	return image;
}


GLuint ModuleTexture::FullLoadFromRoute(const char* rute_image_file_name) {
	GLuint textureID=0;
	ScratchImage sImage;
	wchar_t* w_image = new wchar_t[strlen(rute_image_file_name) + 1];
	mbstowcs_s(NULL, w_image, strlen(rute_image_file_name) + 1, rute_image_file_name, strlen(rute_image_file_name));


	HRESULT res = LoadFromDDSFile(w_image, DDS_FLAGS_NONE, nullptr, sImage);
	
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