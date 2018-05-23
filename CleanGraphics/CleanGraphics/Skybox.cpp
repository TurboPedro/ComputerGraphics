#include "Skybox.h"

#include <iostream>
#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <SOIL/SOIL.h>


Skybox::Skybox(float _size, std::string _directory, std::string _extension, AGeometry::MaterialInfo info) :
	AGeometry(info)
{
	extension = _extension;
	if (_directory.find_last_of('/') != _directory.length() - 1 && _directory.length() > 0) {
		directory = _directory + "/";
	}
	else {
		directory = _directory;
	}
	size = _size;
	loaded = true;
	setup();
}

Skybox::~Skybox()
{
}

GLuint Skybox::GetElementCount() const
{
	return 12 * 3;
}

GLenum Skybox::GetElementType() const
{
	return GL_UNSIGNED_INT;
}

GLuint Skybox::GetTextureId() const
{
	return texId;
}

bool Skybox::IsLoaded() const
{
	return loaded;
}

void Skybox::setup()
{
	float side = size;
	float side2 = size / 2.0f;
	float v[24 * 3] = {
		// Front
		-side2, -side2, side2,
		side2, -side2, side2,
		side2,  side2, side2,
		-side2,  side2, side2,
		// Right
		side2, -side2, side2,
		side2, -side2, -side2,
		side2,  side2, -side2,
		side2,  side2, side2,
		// Back
		-side2, -side2, -side2,
		-side2,  side2, -side2,
		side2,  side2, -side2,
		side2, -side2, -side2,
		// Left
		-side2, -side2, side2,   //12
		-side2,  side2,  side2,   //13
		-side2,  side2, -side2,  //14
		-side2, -side2, -side2,  //15
		// Bottom
		-side2, -side2, side2,
		-side2, -side2, -side2,
		side2, -side2, -side2,
		side2, -side2, side2,
		// Top
		-side2,  side2, side2,
		side2,  side2, side2,
		side2,  side2, -side2,
		-side2,  side2, -side2
	};

	GLuint el[] = {
		0,2,1,    0,3,2,    4,6,5,     4,7,6,
		8,10,9,   8,11,10,  12,14,13,  12,15,14,
		16,18,17, 16,19,18, 20,22,21,  20,23,22
	};

	//Create vao
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//create vbo for vertices
	glGenBuffers(1, &VertexVBO);
	glBindBuffer(GL_ARRAY_BUFFER, VertexVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 3 * 24, v, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0,					// attribute
		3,					// number of elements per vertex, here (x,y,z)
		GL_FLOAT,			// the type of each element
		GL_FALSE,			// take our values as-is
		0,					// no extra data between each position
		0					// offset of first element
	);
	glEnableVertexAttribArray(0);

	//create IBO
	glGenBuffers(1, &IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 3 * 12, el, GL_STATIC_DRAW);

	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &texId);  //set the texID as a member variable
	glBindTexture(GL_TEXTURE_CUBE_MAP, texId);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	const char * suffixes[] = { "right", "left", "top", "down", "front", "back" };

	GLuint targets[] = {
		GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for (int i = 0; i < 6; i++) {
		int channel;
		int width, height;
		std::string filename;
		filename = directory + std::string(suffixes[i]) + extension;
		unsigned char * image = SOIL_load_image(filename.c_str(), &width, &height, &channel, SOIL_LOAD_RGB);
		if (!image) {
			std::cerr << "Could not load image " << filename.c_str() << std::endl;
			loaded = false;
		}
		glTexImage2D(targets[i], 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		glGenerateMipmap(GL_TEXTURE_2D);
		SOIL_free_image_data(image);
	}

}