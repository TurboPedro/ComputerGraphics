#include "Sphere.h"

#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <SOIL/SOIL.h>

Sphere::Sphere(float rad, GLuint sl, GLuint st, AGeometry::MaterialInfo info)
	: AGeometry(info)
{
	radius = rad;
	slices = sl;
	stacks = st;
	setup();
}

Sphere::~Sphere()
{
}

void Sphere::setup()
{
	nVerts = (slices + 1) * (stacks + 1);
	elements = (slices * 2 * (stacks - 1)) * 3;

	// Verts
	float * v = new float[3 * nVerts];
	// Normals
	float * n = new float[3 * nVerts];
	// Tex coords
	float * tex = new float[2 * nVerts];
	// Elements
	unsigned int * el = new unsigned int[elements];

	// Generate the vertex data
	generateVertexes(v, n, tex, el);

	//Create vao
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//create vbo for vertices
	glGenBuffers(1, &VertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * nVerts, v, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,					// attribute
		3,					// number of elements per vertex, here (x,y,z)
		GL_FLOAT,			// the type of each element
		GL_FALSE,			// take our values as-is
		0,					// no extra data between each position
		0					// offset of first element
	);
	glEnableVertexAttribArray(0);

	//create vbo for normals
	glGenBuffers(1, &NormalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, NormalVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * nVerts, n, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1,					// attribute
		3,					// number of elements per vertex, here (x,y,z)
		GL_FLOAT,			// the type of each element
		GL_FALSE,			// take our values as-is
		0,					// no extra data between each position
		0					// offset of first element
	);
	glEnableVertexAttribArray(1);

	//create vbo for textures
	glGenBuffers(1, &TextureVBO);
	glBindBuffer(GL_ARRAY_BUFFER, TextureVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 2 * nVerts, tex, GL_STATIC_DRAW);
	glVertexAttribPointer(
		2,					// attribute
		2,                 // number of elements per vertex, here (x,y)
		GL_FLOAT,          // the type of each element
		GL_FALSE,          // take our values as-is
		0,                 // no extra data between each position
		0                  // offset of first element
	);
	glEnableVertexAttribArray(2);

	//create IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * elements, el, GL_STATIC_DRAW);
}

GLuint Sphere::GetElementCount() const
{
	return elements;
}

GLenum Sphere::GetElementType() const
{
	return GL_UNSIGNED_INT;
}

void Sphere::generateVertexes(float * pos, float * norm, float * tex, unsigned int * el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / slices;
	GLfloat phiFac = glm::pi<float>() / stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for (GLuint i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / slices;
		for (GLuint j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			pos[idx] = radius * nx;
			pos[idx + 1] = radius * ny;
			pos[idx + 2] = radius * nz;
			norm[idx] = nx;
			norm[idx + 1] = ny;
			norm[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (GLuint i = 0; i < slices; i++) {
		GLuint stackStart = i * (stacks + 1);
		GLuint nextStackStart = (i + 1) * (stacks + 1);
		for (GLuint j = 0; j < stacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == stacks - 1) {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}
}