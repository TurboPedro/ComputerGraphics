#include "AShader.h"



AShader::AShader()
{
}


AShader::~AShader()
{
}

bool AShader::use(LoadedModel *loadedModel, std::map<const char *, ALight *> *lights, SModelViewProjection *mvp, Texture *diffuseTexture, Texture *specularTexture)
{
	std::vector<Mesh> *meshes = loadedModel->GetMeshes();
	unsigned int i = 0;

	for (auto &mesh : *meshes) {
		if (!use(&mesh, lights, mvp))
			break;
		i++;
	}

	return (i == meshes->size());
}

bool AShader::use(AGeometry * object, SModelViewProjection * mvp, Skybox * skybox)
{
	return false;
}

bool AShader::use(LoadedModel * loadedModel, SModelViewProjection * mvp, Skybox * skybox)
{
	std::vector<Mesh> *meshes = loadedModel->GetMeshes();
	unsigned int i = 0;

	for (auto &mesh : *meshes) {
		if (!use(&mesh, mvp, skybox))
			break;
		i++;
	}

	return (i == meshes->size());
}
