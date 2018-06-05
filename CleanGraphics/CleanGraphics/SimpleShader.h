#pragma once
#include "AShader.h"

#include <GL/glew.h>
#include <GL/GL.h>
#include <glm/mat4x4.hpp>

class SimpleShader :
	public AShader
{
public:
	SimpleShader();
	~SimpleShader();

	virtual bool use(AGeometry *object, std::map<const char *, ALight *> *lights, AShader::SModelViewProjection *mvp, Texture *diffuseTexture, Texture *specularTexture);
};

