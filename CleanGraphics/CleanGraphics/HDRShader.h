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

	virtual bool use(AGeometry *object = NULL, ALight *light = NULL, SModelViewProjection *mvp = NULL, Texture *diffuseTexture = NULL, Texture *specularTexture = NULL);
	void beforeDraw();
};

