#include "TextureShader.h"

#include <glm/gtc/type_ptr.hpp>

TextureShader::TextureShader()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("texture.vert", "texture.frag");

	shaderProgram->addUniform("Tex1");
	shaderProgram->addUniform("Tex2");

	shaderProgram->addUniform("Material.Shiness");
	shaderProgram->addUniform("Material.Alpha");
	shaderProgram->addUniform("Material.Ka");
	shaderProgram->addUniform("Material.Kd");
	shaderProgram->addUniform("Material.Ks");

	shaderProgram->addUniform("Light.Position");
	shaderProgram->addUniform("Light.La");
	shaderProgram->addUniform("Light.Ld");
	shaderProgram->addUniform("Light.Ls");

	shaderProgram->addUniform("ModelViewMatrix");
	shaderProgram->addUniform("NormalMatrix");
	shaderProgram->addUniform("mvp");
}


TextureShader::~TextureShader()
{
}

bool TextureShader::use(AGeometry *object, ALight *light, AShader::SModelViewProjection *mvp, Texture *diffuseTexture = NULL, Texture *specularTexture = NULL)
{
	if (light->GetType() != ALight::SIMPLE)
		return (false);

	AGeometry::MaterialInfo material = object->GetMaterialInfo();
	SColor lightColor = light->GetColor();

	glm::mat4 modelView = mvp->view * mvp->model;
	glm::mat4 inverseModelView = glm::inverse(modelView);
	glm::mat3 normalMatrix = glm::transpose(inverseModelView);
	glm::mat4 mvpMatrix = mvp->projection * mvp->view * mvp->model;
	shaderProgram->use();

	glEnable(GL_TEXTURE_2D);

	if (diffuseTexture && diffuseTexture->Loaded()) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture->GetID());
		glUniform1i(shaderProgram->uniform("Tex1"), 0);
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(shaderProgram->uniform("Tex1"), 0);
	}
	if (specularTexture && specularTexture->Loaded()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTexture->GetID());
		glUniform1i(shaderProgram->uniform("Tex2"), 1);
	}

	glBindVertexArray(object->GetVAO());
	glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(modelView * light->GetPosition()));
	glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(lightColor.Ambient));
	glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(lightColor.Diffuse));
	glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(lightColor.Specular));
	glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &(material.color.Shiness));
	glUniform1fv(shaderProgram->uniform("Material.Alpha"), 1, &(material.Alpha));
	glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(material.color.Ambient));
	glUniform3fv(shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(material.color.Diffuse));
	glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(material.color.Specular));
	glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glDrawElements(GL_TRIANGLES, object->GetElementCount(), object->GetElementType(), 0);

//	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	shaderProgram->disable();
	return true;
}

bool TextureShader::use(LoadedModel *loadedModel, ALight *light, AShader::SModelViewProjection *mvp, Texture *diffuseTexture = NULL, Texture *specularTexture = NULL)
{
	std::vector<Mesh> *meshes = loadedModel->GetMeshes();
	unsigned int i = 0;

	for (auto &mesh : *meshes) {
		Texture *loadedDiffuseTexture = diffuseTexture;
		Texture *loadedSpecularTexture = specularTexture;

		for (auto &texture : *(mesh.GetTextures())) {
			if (texture->Loaded()) {
				if (texture->GetType() == Texture::DIFFUSE && !loadedDiffuseTexture)
					loadedDiffuseTexture = texture;
				else if (!loadedSpecularTexture)
					loadedSpecularTexture = texture;
			}
		}
		if (!use(&mesh, light, mvp, loadedDiffuseTexture, loadedSpecularTexture))
			break;
		i++;
	}

	return (i == meshes->size());
}
