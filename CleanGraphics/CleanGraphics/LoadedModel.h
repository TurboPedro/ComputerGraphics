#pragma once

#include <GL\glew.h>
#include <assimp\scene.h>

#include "Mesh.h"

class LoadedModel
{
protected:
	std::vector<Mesh> meshes;
	std::vector<Texture *> textures_loaded;
	std::string directory;

	void loadModel(std::string path);
	void processNode(aiNode *node, const aiScene *scene);
	Mesh processMesh(aiMesh *mesh, const aiScene *scene);
	std::vector<Texture *> loadMaterialTextures(aiMaterial *mat, aiTextureType type, Texture::ETextureType typeName);

public:

	LoadedModel(const GLchar *path);
	~LoadedModel();

	std::vector<Mesh> *GetMeshes();
};

