#pragma once

#include "AShader.h"
#include "Texture.h"

class NormalMapShader :
	public AShader
{
public:
	NormalMapShader();
	~NormalMapShader();

	virtual bool use(AGeometry *object, ALight *light, AShader::SModelViewProjection *mvp, Texture *diffuseTexture, Texture *normalMapTexture);
	virtual bool use(LoadedModel *loadedModel, ALight *light, AShader::SModelViewProjection *mvp, Texture *diffuseTexture, Texture *normalMapTexture);
};

