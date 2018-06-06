#include "LightBulbShader.h"

#include <glm//gtc/constants.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

LightBulbShader::LightBulbShader()
{
	lightName = "";

	shaderProgram = new ShaderProgram();
	shaderProgram->initFromFiles("lightbulb.vert", "lightbulb.frag");

	shaderProgram->addUniform("LightColor");
	shaderProgram->addUniform("mvp");
}


LightBulbShader::~LightBulbShader()
{
}

bool LightBulbShader::use(AGeometry *object, std::map<const char *, ALight *> *lights, AShader::SModelViewProjection *mvp, Texture *diffuseTexture = NULL, Texture *specularTexture = NULL)
{
	if (lights->find(lightName) == lights->end()) {
		std::cout << " Couldn't find light " << lightName << std::endl;
		return (false);
	}

	ALight *light = (*lights)[lightName];

	SColor lightColor = light->GetColor();

	glm::mat4 mvpMatrix = mvp->projection * mvp->view * mvp->model;
	shaderProgram->use();

	glBindVertexArray(object->GetVAO());
	glUniform3fv(shaderProgram->uniform("LightColor"), 1, glm::value_ptr(lightColor.Ambient));
	glUniformMatrix4fv(shaderProgram->uniform("mvp"), 1, GL_FALSE, glm::value_ptr(mvpMatrix));

	if (object->UseIBO()) {
		glDrawElements(GL_TRIANGLES, object->GetElementCount(), object->GetElementType(), 0);
	}
	else {
		glDrawArrays(GL_TRIANGLES, 0, object->GetElementCount());
	}

	shaderProgram->disable();
	glBindVertexArray(0);
	return true;
}

void LightBulbShader::setLight(const char *str)
{
	lightName = str;
}
