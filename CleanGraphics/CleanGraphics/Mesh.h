#pragma once
#include "AGeometry.h"
#include "Vertex.h"
#include "Texture.h"

#include <vector>

class Mesh :
	public AGeometry
{
protected:
	std::vector<SVertex> vertices;
	std::vector<Texture *> textures;
	std::vector<GLuint> indices;

	virtual void setup();

public:

	Mesh(std::vector<SVertex> _vertices, std::vector<Texture *> _textures, std::vector<GLuint> _indices);
	~Mesh();

	virtual GLuint GetElementCount() const;
	virtual GLenum GetElementType() const;
	virtual std::vector<Texture *> *GetTextures();
};