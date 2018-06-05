#pragma once
#include "AShader.h"
class SkyboxShader :
	public AShader
{
public:
	SkyboxShader();
	~SkyboxShader();

	virtual bool use(AGeometry *object, SModelViewProjection *mvp, Skybox *skybox);
	virtual bool use(AGeometry *object, std::map<const char *, ALight *> *lights, SModelViewProjection *mvp, Texture *diffuseTexture = NULL, Texture *specularTexture = NULL);
};

