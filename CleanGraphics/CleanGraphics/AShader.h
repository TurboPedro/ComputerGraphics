#pragma once

#ifndef __A_SHADER
# define __A_SHADER

#include <GL\glew.h>
#include <glm\glm.hpp>

#include "Loader.h"
#include "AGeometry.h"
#include "ALight.h"
#include "LoadedModel.h"
#include "Skybox.h"

class AShader
{

protected:
	ShaderProgram *shaderProgram;

public:

	struct SModelViewProjection {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 projection;
	};

	AShader();
	~AShader();

	virtual bool use(AGeometry *object, std::map<const char *, ALight *> *lights, SModelViewProjection *mvp, Texture *diffuseTexture = NULL, Texture *specularTexture = NULL) = 0;
	virtual bool use(LoadedModel *loadedModel, std::map<const char *, ALight *> *lights, SModelViewProjection *mvp, Texture *diffuseTexture = NULL, Texture *specularTexture = NULL);
	virtual bool use(AGeometry *object, SModelViewProjection *mvp, Skybox *skybox);
	virtual bool use(LoadedModel *loadedModel, SModelViewProjection *mvp, Skybox *skybox);
};

#endif // !__A_SHADER