#pragma once
#include "AShader.h"
class HDRShader :
	public AShader
{

	GLuint FBO, RBO, VAO, VertexVBO, TextureVBO;
	GLuint TBOs[2], pingpongFBOs[2], pingpongTBOs[2];
	int width, height;
	ShaderProgram *blurShader;

public:
	HDRShader(int w, int h);
	~HDRShader();

	virtual bool use(AGeometry *object = NULL, std::map<const char *, ALight *> *lights = NULL, SModelViewProjection *mvp = NULL, Texture *diffuseTexture = NULL, Texture *specularTexture = NULL);
	void beforeDraw();
};

