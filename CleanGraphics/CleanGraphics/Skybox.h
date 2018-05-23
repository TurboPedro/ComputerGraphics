#pragma once

#include "AGeometry.h"

#include <string>

class Skybox :
	public AGeometry
{

protected:

	std::string directory;
	std::string extension;
	bool loaded;
	float size;
	GLuint texId;

	virtual void setup();

public:
	Skybox(float _size, std::string _directory, std::string _extension, AGeometry::MaterialInfo info);
	~Skybox();

	virtual GLuint GetElementCount() const;
	virtual GLenum GetElementType() const;
	GLuint GetTextureId() const;
	bool IsLoaded() const;
};