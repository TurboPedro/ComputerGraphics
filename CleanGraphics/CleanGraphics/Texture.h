#pragma once

#include <string>
#include <GL\glew.h>
#include <assimp\scene.h>

class Texture {
public:
	enum ETextureType {
		DIFFUSE = 0,
		SPECULAR
	};

	Texture(const char *path, const char *directory, ETextureType type);
	~Texture();

	GLuint GetID() const;
	ETextureType GetType() const;
	const std::string &GetPath() const;
	bool Loaded() const;

protected:
	bool loaded;
	GLuint id;
	ETextureType type;
	std::string path;
};

GLint TextureFromFile(const char *path, std::string directory);