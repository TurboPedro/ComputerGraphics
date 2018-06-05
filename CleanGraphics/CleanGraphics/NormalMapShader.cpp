#include "NormalMapShader.h"

#include <glm/gtc/type_ptr.hpp>

NormalMapShader::NormalMapShader()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("normalmap.vert", "normalmap.frag");

	shaderProgram->addUniform("ColorTex");
	shaderProgram->addUniform("NormalMapTex");

	shaderProgram->addUniform("Material.Shiness");
	shaderProgram->addUniform("Material.Alpha");
	shaderProgram->addUniform("Material.Ka");
	shaderProgram->addUniform("Material.Ks");

	shaderProgram->addUniform("Light.Position");
	shaderProgram->addUniform("Light.La");
	shaderProgram->addUniform("Light.Ld");
	shaderProgram->addUniform("Light.Ls");

	shaderProgram->addUniform("ModelViewMatrix");
	shaderProgram->addUniform("NormalMatrix");
	shaderProgram->addUniform("mvp");
}


NormalMapShader::~NormalMapShader()
{
}

bool NormalMapShader::use(AGeometry *object, std::map<const char *, ALight *> *lights, AShader::SModelViewProjection *mvp, Texture *normalMapTexture, Texture *diffuseTexture = NULL)
{
	ALight *light = lights->begin()->second;

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
		glActiveTexture(GL_TEXTURE0 + diffuseTexture->GetID());
		glBindTexture(GL_TEXTURE_2D, diffuseTexture->GetID());
		glUniform1i(shaderProgram->uniform("ColorTex"), 0 + diffuseTexture->GetID());
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(shaderProgram->uniform("ColorTex"), 0);
	}
	if (normalMapTexture && normalMapTexture->Loaded()) {
		glActiveTexture(GL_TEXTURE1 + normalMapTexture->GetID());
		glBindTexture(GL_TEXTURE_2D, normalMapTexture->GetID());
		glUniform1i(shaderProgram->uniform("NormalMapTex"), 1 + normalMapTexture->GetID());
	}

	glBindVertexArray(object->GetVAO());
	glUniform4fv(shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(modelView * light->GetPosition()));
	glUniform3fv(shaderProgram->uniform("Light.La"), 1, glm::value_ptr(lightColor.Ambient));
	glUniform3fv(shaderProgram->uniform("Light.Ld"), 1, glm::value_ptr(lightColor.Diffuse));
	glUniform3fv(shaderProgram->uniform("Light.Ls"), 1, glm::value_ptr(lightColor.Specular));
	glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &(material.color.Shiness));
	glUniform1fv(shaderProgram->uniform("Material.Alpha"), 1, &(material.Alpha));
	glUniform3fv(shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(material.color.Ambient));
	glUniform3fv(shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(material.color.Specular));
	glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glDrawElements(GL_TRIANGLES, object->GetElementCount(), object->GetElementType(), 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	shaderProgram->disable();
	return true;
}

bool NormalMapShader::use(LoadedModel *loadedModel, std::map<const char *, ALight *> *lights, AShader::SModelViewProjection *mvp, Texture *normalMapTexture, Texture *diffuseTexture = NULL)
{
	std::vector<Mesh> *meshes = loadedModel->GetMeshes();
	unsigned int i = 0;

	for (auto &mesh : *meshes) {
		Texture *loadedDiffuseTexture = diffuseTexture;
		Texture *loadedSpecularTexture = normalMapTexture;

		for (auto &texture : *(mesh.GetTextures())) {
			if (texture->Loaded()) {
				if (texture->GetType() == Texture::DIFFUSE && !loadedDiffuseTexture)
					loadedDiffuseTexture = texture;
				else if (!loadedSpecularTexture)
					loadedSpecularTexture = texture;
			}
		}
		if (!use(&mesh, lights, mvp, loadedDiffuseTexture, loadedSpecularTexture))
			break;
		i++;
	}

	return (i == meshes->size());
}