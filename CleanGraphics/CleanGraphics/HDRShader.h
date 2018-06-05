#pragma once
#include "AShader.h"
class HDRShader :
	public AShader
{

	GLuint FBO, RBO, TBO, VAO, VertexVBO, TextureVBO;
	int width, height;

public:
	HDRShader(int w, int h);
	~HDRShader();

	virtual bool use(AGeometry *object = NULL, std::map<const char *, ALight *> *lights = NULL, SModelViewProjection *mvp = NULL, Texture *diffuseTexture = NULL, Texture *specularTexture = NULL);
	void beforeDraw();
};

