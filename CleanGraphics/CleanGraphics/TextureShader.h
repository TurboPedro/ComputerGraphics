#pragma once

#include "AShader.h"
#include "Texture.h"

class TextureShader :
	public AShader
{
public:
	TextureShader();
	~TextureShader();

	virtual bool use(AGeometry *object, ALight *light, AShader::SModelViewProjection *mvp, Texture *diffuseTexture, Texture *specularTexture);
	virtual bool use(LoadedModel *loadedModel, ALight *light, AShader::SModelViewProjection *mvp, Texture *diffuseTexture, Texture *specularTexture);
};

