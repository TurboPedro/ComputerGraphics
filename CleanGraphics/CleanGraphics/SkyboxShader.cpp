#include "SkyboxShader.h"

#include <glm/gtc/type_ptr.hpp>

SkyboxShader::SkyboxShader()
{
	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("skybox.vert", "skybox.frag");

	shaderProgram->addUniform("CubeMapTex");

	shaderProgram->addUniform("mvp");
}


SkyboxShader::~SkyboxShader()
{
}

bool SkyboxShader::use(AGeometry * object, SModelViewProjection * mvp, Skybox * skybox)
{
	AGeometry::MaterialInfo material = object->GetMaterialInfo();

	glm::mat4 modelView = mvp->view * mvp->model;
	glm::mat4 inverseModelView = glm::inverse(modelView);
	glm::mat3 normalMatrix = glm::transpose(inverseModelView);
	glm::mat4 mvpMatrix = mvp->projection * mvp->view * mvp->model;
	shaderProgram->use();

	glEnable(GL_TEXTURE_2D);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, skybox->GetTextureId());
	glUniform1i(shaderProgram->uniform("CubeMapTex"), 0);

	glBindVertexArray(skybox->GetVAO());
	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));
	glDrawElements(GL_TRIANGLES, object->GetElementCount(), object->GetElementType(), 0);

	//	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
	shaderProgram->disable();
	return true;
}

bool SkyboxShader::use(AGeometry * object, ALight * light, SModelViewProjection * mvp, Texture * diffuseTexture, Texture * specularTexture)
{
	return false;
}
