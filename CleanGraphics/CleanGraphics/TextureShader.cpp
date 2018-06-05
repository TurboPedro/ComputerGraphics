#include "TextureShader.h"

#include <glm/gtc/type_ptr.hpp>

TextureShader::TextureShader()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("texture.vert", "texture.frag");

	shaderProgram->addUniform("DiffuseTexture");
	shaderProgram->addUniform("SpecularTexture");

	shaderProgram->addUniform("Material.Shiness");
	shaderProgram->addUniform("Material.Alpha");

	for (int i = 0; i < 8; i++) {
		std::string number = std::to_string(i);
		shaderProgram->addUniform(("Lights[" + number + "].Position").c_str());
		shaderProgram->addUniform(("Lights[" + number + "].La").c_str());
		shaderProgram->addUniform(("Lights[" + number + "].Ld").c_str());
		shaderProgram->addUniform(("Lights[" + number + "].Ls").c_str());
		shaderProgram->addUniform(("Lights[" + number + "].Constant").c_str());
		shaderProgram->addUniform(("Lights[" + number + "].Linear").c_str());
		shaderProgram->addUniform(("Lights[" + number + "].Quadratic").c_str());
	}

	shaderProgram->addUniform("ModelViewMatrix");
	shaderProgram->addUniform("NormalMatrix");
	shaderProgram->addUniform("mvp");
}


TextureShader::~TextureShader()
{
}

bool TextureShader::use(AGeometry *object, std::map<const char *, ALight *> *lights, AShader::SModelViewProjection *mvp, Texture *diffuseTexture = NULL, Texture *specularTexture = NULL)
{
	//ALight *light = lights->begin()->second;

	//if (light->GetType() != ALight::SIMPLE)
	//	return (false);

	AGeometry::MaterialInfo material = object->GetMaterialInfo();
	//SColor lightColor = light->GetColor();
	//SAttenuation attenuation = light->GetAttenuation();

	glm::mat4 modelView = mvp->view * mvp->model;
	glm::mat4 inverseModelView = glm::inverse(modelView);
	glm::mat3 normalMatrix = glm::transpose(inverseModelView);
	glm::mat4 mvpMatrix = mvp->projection * mvp->view * mvp->model;
	shaderProgram->use();

	glEnable(GL_TEXTURE_2D);

	if (diffuseTexture && diffuseTexture->Loaded()) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, diffuseTexture->GetID());
		glUniform1i(shaderProgram->uniform("DiffuseTexture"), 0);
	}
	else {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUniform1i(shaderProgram->uniform("DiffuseTexture"), 0);
	}
	if (specularTexture && specularTexture->Loaded()) {
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, specularTexture->GetID());
		glUniform1i(shaderProgram->uniform("SpecularTexture"), 1);
	}

	glBindVertexArray(object->GetVAO());
	int k = 0;
	int max = (lights->size() > 8 ? 8 : lights->size());
	for (auto &i : *lights) {
		ALight *light = i.second;
		SColor lightColor = light->GetColor();
		SAttenuation attenuation = light->GetAttenuation();
		std::string number = std::to_string(k);

		glUniform4fv(shaderProgram->uniform(("Lights[" + number + "].Position").c_str()), 1, glm::value_ptr(mvp->view * light->GetPosition()));
		glUniform3fv(shaderProgram->uniform(("Lights[" + number + "].La").c_str()), 1, glm::value_ptr(lightColor.Ambient));
		glUniform3fv(shaderProgram->uniform(("Lights[" + number + "].Ld").c_str()), 1, glm::value_ptr(lightColor.Diffuse));
		glUniform3fv(shaderProgram->uniform(("Lights[" + number + "].Ls").c_str()), 1, glm::value_ptr(lightColor.Specular));
		glUniform1fv(shaderProgram->uniform(("Lights[" + number + "].Constant").c_str()), 1, &(attenuation.constant));
		glUniform1fv(shaderProgram->uniform(("Lights[" + number + "].Linear").c_str()), 1, &(attenuation.linear));
		glUniform1fv(shaderProgram->uniform(("Lights[" + number + "].Quadratic").c_str()), 1, &(attenuation.quadratic));

		if (k >= max)
			break;
		k++;
	}
	glUniform1fv(shaderProgram->uniform("Material.Shiness"), 1, &(material.color.Shiness));
	glUniform1fv(shaderProgram->uniform("Material.Alpha"), 1, &(material.Alpha));
	glUniformMatrix4fv(shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(modelView));
	glUniformMatrix3fv(shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normalMatrix));
	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	if (object->UseIBO())
		glDrawElements(GL_TRIANGLES, object->GetElementCount(), object->GetElementType(), 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, object->GetElementCount());

//	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	shaderProgram->disable();
	return true;
}

bool TextureShader::use(LoadedModel *loadedModel, std::map<const char *, ALight *> *lights, AShader::SModelViewProjection *mvp, Texture *diffuseTexture = NULL, Texture *specularTexture = NULL)
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
		if (!use(&mesh, lights, mvp, loadedDiffuseTexture, loadedSpecularTexture))
			break;
		i++;
	}

	return (i == meshes->size());
}
