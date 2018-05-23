#pragma once
#include "AGeometry.h"

class Sphere :
	public AGeometry
{
protected:
	GLuint nVerts, elements;
	float radius;
	GLuint slices, stacks;

	void generateVertexes(float *pos, float *norm, float *tex, unsigned int *el);
	virtual void setup();

public:
	Sphere(float rad, GLuint sl, GLuint st, AGeometry::MaterialInfo info);
	~Sphere();

	virtual GLuint GetElementCount() const;
	virtual GLenum GetElementType() const;
};

