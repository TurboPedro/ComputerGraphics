#pragma once

#include "AShader.h"
#include "Texture.h"

class NormalMapShader :
	public AShader
{
public:
	NormalMapShader();
	~NormalMapShader();

	virtual bool use(AGeometry *object, std::map<const char *, ALight *> *lights, AShader::SModelViewProjection *mvp, Texture *diffuseTexture, Texture *normalMapTexture);
	virtual bool use(LoadedModel *loadedModel, std::map<const char *, ALight *> *lights, AShader::SModelViewProjection *mvp, Texture *diffuseTexture, Texture *normalMapTexture);
};

