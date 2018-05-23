#include "Texture.h"

#include <iostream>
#include <SOIL\SOIL.h>

Texture::Texture(const char *texturePath, const char *directory = "", Texture::ETextureType textureType = Texture::DIFFUSE)
{
	path = std::string(texturePath);
	std::string fullPath;
	if (directory[0]) {
		fullPath = directory;
		fullPath = fullPath + "/" + path;
	}
	else
		fullPath = path;
	type = textureType;
	loaded = true;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, channels;
	unsigned char *image = SOIL_load_image(fullPath.c_str(), &width, &height, &channels, SOIL_LOAD_AUTO);
	if (!image) {
		std::cerr << "Failed to load image from " << fullPath << std::endl;
		loaded = false;
		return ;
	}

	if (channels == 3)
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	else
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);

	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
}

GLuint Texture::GetID() const
{
	return id;
}

Texture::ETextureType Texture::GetType() const
{
	return type;
}

const std::string &Texture::GetPath() const
{
	return path;
}

bool Texture::Loaded() const
{
	return loaded;
}
