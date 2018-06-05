#pragma once

#include "AShader.h"
#include "Texture.h"

class TextureShader :
	public AShader
{
public:
	TextureShader();
	~TextureShader();

	virtual bool use(AGeometry *object, std::map<const char *, ALight *> *lights, AShader::SModelViewProjection *mvp, Texture *diffuseTexture, Texture *specularTexture);
	virtual bool use(LoadedModel *loadedModel, std::map<const char *, ALight *> *lights, AShader::SModelViewProjection *mvp, Texture *diffuseTexture, Texture *specularTexture);
};

