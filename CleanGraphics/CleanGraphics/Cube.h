#pragma once
#include "AGeometry.h"

class Cube :
	public AGeometry
{

	GLint elementSize;

	virtual void setup();
	static glm::vec3 computeNormal(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);

public:

	Cube(AGeometry::MaterialInfo info);
	~Cube();

	virtual GLuint GetElementCount() const;
	virtual GLuint GetElementType() const;
};

