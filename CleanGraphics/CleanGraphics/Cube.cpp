#include "Cube.h"

#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <vector>
#include <iostream>
#include <SOIL/SOIL.h>

Cube::Cube(AGeometry::MaterialInfo info) :
	AGeometry(info)
{
	hasIbo = false;
	elementSize = 0;
	setup();
}

Cube::~Cube()
{
}

glm::vec3 Cube::computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
	return glm::cross(p2 - p1, p3 - p1);
}

void Cube::setup()
{
	GLfloat cube_vertices[] = {
		// Faces are grouped by 2
		// 1
		-1.0f,	-1.0f,	-1.0f,
		-1.0f,	-1.0f,	1.0f,
		-1.0f,	1.0f,	1.0f,
		// 2
		-1.0f,	-1.0f,	-1.0f,
		-1.0f,	1.0f,	1.0f,
		-1.0f,	1.0f,	-1.0f,
		// 3
		-1.0f,	-1.0f,	-1.0f,
		-1.0f,	1.0f,	-1.0f,
		1.0f,	1.0f,	-1.0f,
		// 4
		-1.0f,	-1.0f,	-1.0f,
		1.0f,	1.0f,	-1.0f,
		1.0f,	-1.0f,	-1.0f,
		// 5 modified
		-1.0f,	-1.0f,	-1.0f,
		1.0f,	-1.0f,	1.0f,
		-1.0f,	-1.0f,	1.0f,
		// 6 modified
		-1.0f,	-1.0f,	-1.0f,
		1.0f,	-1.0f,	-1.0f,
		1.0f,	-1.0f,	1.0f,

		// 7 modified
		1.0f,	-1.0f,	-1.0f,
		1.0f,	1.0f,	1.0f,
		1.0f,	-1.0f,	1.0f,
		// 8 modified
		1.0f,	-1.0f,	-1.0f,
		1.0f,	1.0f,	-1.0f,
		1.0f,	1.0f,	1.0f,
		// 9 modified
		-1.0f,	-1.0f,	1.0f,
		1.0f,	1.0f,	1.0f,
		-1.0f,	1.0f,	1.0f,
		// 10 modified
		-1.0f,	-1.0f,	1.0f,
		1.0f,	-1.0f,	1.0f,
		1.0f,	1.0f,	1.0f,
		// 11
		-1.0f,	1.0f,	-1.0f,
		-1.0f,	1.0f,	1.0f,
		1.0f,	1.0f,	1.0f,
		// 12
		-1.0f,	1.0f,	-1.0f,
		1.0f,	1.0f,	1.0f,
		1.0f,	1.0f,	-1.0f,

	};

	std::vector<glm::vec3> cube_normal;

	int k = 0;
	glm::vec3 normale;
	while (k < 3 * 36) {
		normale = computeNormal(glm::vec3(cube_vertices[k], cube_vertices[k + 1], cube_vertices[k + 2]),
			glm::vec3(cube_vertices[k + 3], cube_vertices[k + 4], cube_vertices[k + 5]),
			glm::vec3(cube_vertices[k + 6], cube_vertices[k + 7], cube_vertices[k + 8]));
		cube_normal.push_back(normale);
		cube_normal.push_back(normale);
		cube_normal.push_back(normale);
		k = k + 9;
	}

	GLfloat cube_colors[] = {
		// front colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
		// back colors
		1.0, 0.0, 0.0,
		0.0, 1.0, 0.0,
		0.0, 0.0, 1.0,
		1.0, 1.0, 1.0,
	};

	float tex[36 * 2] = {
		// 1
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		// 2
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		// 3
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		// 4
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
		// 5 modified
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// 6 modified
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		// 7 modified
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// 8 modified
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		// 9 modified
		0.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
		// 10 modified
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		// 11
		0.0f, 0.0f,
		0.0f, 1.0f,
		1.0f, 1.0f,
		// 12
		0.0f, 0.0f,
		1.0f, 1.0f,
		1.0f, 0.0f,
	};

	//create vao
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//create vbo for vertices
	glGenBuffers(1, &VertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 36, &cube_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // attribute
		3,                 // number of elements per vertex, here (x,y,z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);
	glEnableVertexAttribArray(0);

	//create vbo for normales
	glGenBuffers(1, &NormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 36, cube_normal.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // attribute
		3,                 // number of elements per vertex, here (x,y,z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);
	glEnableVertexAttribArray(1);

	//create vbo for textures
	glGenBuffers(1, &TextureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TextureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * 36, &tex, GL_STATIC_DRAW);
	glVertexAttribPointer(
		2, // attribute
		2,                 // number of elements per vertex, here (x,y,z)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);
	glEnableVertexAttribArray(2);

	glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &elementSize);

	glBindVertexArray(0);
}

GLuint Cube::GetElementCount() const
{
	return elementSize;
}

GLuint Cube::GetElementType() const
{
	return GL_FLOAT;
}
