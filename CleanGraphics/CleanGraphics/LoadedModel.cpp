#include "LoadedModel.h"

#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

LoadedModel::LoadedModel(const GLchar * path)
{
	loadModel(path);
}

LoadedModel::~LoadedModel()
{
}

std::vector<Mesh> *LoadedModel::GetMeshes()
{
	return &meshes;
}

void LoadedModel::loadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene *scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_OptimizeMeshes | aiProcess_CalcTangentSpace );
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return ;
	}
	if (path.find_last_of('/') == std::string::npos)
		directory = "";
	else
		directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void LoadedModel::processNode(aiNode *node, const aiScene *scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}
	for (GLuint i = 0; i < node->mNumChildren; i++) {
		this->processNode(node->mChildren[i], scene);
	}
}

Mesh LoadedModel::processMesh(aiMesh *mesh, const aiScene *scene)
{
	std::vector<SVertex> vertices;
	std::vector<GLuint> indices;
	std::vector<Texture *> textures;

	for (GLuint i = 0; i < mesh->mNumVertices; i++) {
		SVertex vertex;

		vertex.Position.x = mesh->mVertices[i].x;
		vertex.Position.y = mesh->mVertices[i].y;
		vertex.Position.z = mesh->mVertices[i].z;

		vertex.Normal.x = mesh->mNormals[i].x;
		vertex.Normal.y = mesh->mNormals[i].y;
		vertex.Normal.z = mesh->mNormals[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.TexCoords.x = mesh->mTextureCoords[0][i].x;
			vertex.TexCoords.y = mesh->mTextureCoords[0][i].y;
		}
		else {
			vertex.TexCoords = glm::vec2(0.0f, 0.0f);
		}

		if (mesh->HasTangentsAndBitangents()) {
			vertex.BitTangent.x = mesh->mBitangents[i].x;
			vertex.BitTangent.y = mesh->mBitangents[i].y;
			vertex.BitTangent.z = mesh->mBitangents[i].z;

			vertex.Tangent.x = mesh->mTangents[i].x;
			vertex.Tangent.y = mesh->mTangents[i].y;
			vertex.Tangent.z = mesh->mTangents[i].z;
		}

		vertices.push_back(vertex);
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++) {
		aiFace face = mesh->mFaces[i];
		for (GLuint j = 0; j < face.mNumIndices; j++) {
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<Texture *> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, Texture::ETextureType::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		std::vector<Texture *> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, Texture::ETextureType::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return (Mesh(vertices, textures, indices));
}

std::vector<Texture *> LoadedModel::loadMaterialTextures(aiMaterial *mat, aiTextureType type, Texture::ETextureType typeName)
{
	std::vector<Texture *> textures;
	for (GLuint i = 0; i < mat->GetTextureCount(type); i++) {
		GLboolean skip = false;
		aiString str;
		mat->GetTexture(type, i, &str);
	
		for (GLuint j = 0; j < textures_loaded.size(); j++) {
			if (textures_loaded[j]->GetPath().compare(str.data) == 0) {
				textures.push_back(textures_loaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip) {
			Texture *texture = new Texture(str.C_Str(), directory.c_str(), typeName);
			textures.push_back(texture);
		}
	}
	return textures;
}