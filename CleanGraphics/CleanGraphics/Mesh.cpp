#include "Mesh.h"

Mesh::Mesh(std::vector<SVertex> _vertices, std::vector<Texture *> _textures, std::vector<GLuint> _indices) :
	AGeometry({ 1.0, PremadeColor::White })
{
	vertices = _vertices;
	textures = _textures;
	indices = _indices;
	setup();
}

Mesh::~Mesh()
{
}

void Mesh::setup()
{
	//Create vao
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//create vbo for vertices
	glGenBuffers(1, &VertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SVertex), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,					// attribute
		3,					// number of elements per vertex, here (x,y,z)
		GL_FLOAT,			// the type of each element
		GL_FALSE,			// take our values as-is
		sizeof(SVertex),	// no extra data between each position
		0					// offset of first element
	);
	glEnableVertexAttribArray(0);

	//create vbo for normals
	glGenBuffers(1, &NormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SVertex), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,									// attribute
		3,									// number of elements per vertex, here (x,y,z)
		GL_FLOAT,							// the type of each element
		GL_FALSE,							// take our values as-is
		sizeof(SVertex),					// no extra data between each position
		(GLuint*)offsetof(SVertex, Normal)	// offset of first element
	);
	glEnableVertexAttribArray(1);

	//create vbo for textures
	glGenBuffers(1, &TextureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TextureVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SVertex), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		2,									// attribute
		2,									// number of elements per vertex, here (x,y)
		GL_FLOAT,							// the type of each element
		GL_FALSE,							// take our values as-is
		sizeof(SVertex),					// no extra data between each position
		(GLuint*)offsetof(SVertex, TexCoords)	// offset of first element
	);
	glEnableVertexAttribArray(2);

	//create vbo for tangents
	glGenBuffers(1, &TangentVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TangentVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SVertex), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		3,									// attribute
		3,									// number of elements per vertex, here (x,y,z)
		GL_FLOAT,							// the type of each element
		GL_FALSE,							// take our values as-is
		sizeof(SVertex),					// no extra data between each position
		(GLuint*)offsetof(SVertex, Tangent)	// offset of first element
	);
	glEnableVertexAttribArray(3);

	//create vbo for bit tangents
	glGenBuffers(1, &BitTangentVBO);
	glBindBuffer(GL_ARRAY_BUFFER, BitTangentVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(SVertex), &vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(
		4,									// attribute
		3,									// number of elements per vertex, here (x,y,z)
		GL_FLOAT,							// the type of each element
		GL_FALSE,							// take our values as-is
		sizeof(SVertex),					// no extra data between each position
		(GLuint*)offsetof(SVertex, BitTangent)	// offset of first element
	);
	glEnableVertexAttribArray(4);

	//create IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);
}

GLuint Mesh::GetElementCount() const
{
	return indices.size();
}

GLenum Mesh::GetElementType() const
{
	return GL_UNSIGNED_INT;
}

std::vector<Texture*> *Mesh::GetTextures()
{
	return &textures;
}
