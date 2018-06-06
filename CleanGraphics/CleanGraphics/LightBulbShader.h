#pragma once
#include "AShader.h"
#include <glm/vec3.hpp>

class LightBulbShader :
	public AShader
{

	const char *lightName;

public:
	LightBulbShader();
	~LightBulbShader();

	virtual bool use(AGeometry *object, std::map<const char *, ALight *> *lights, AShader::SModelViewProjection *mvp, Texture *diffuseTexture, Texture *specularTexture);
	void setLight(const char *str);
};

