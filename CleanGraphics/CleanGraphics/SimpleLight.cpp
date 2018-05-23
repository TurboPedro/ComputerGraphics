#include "SimpleLight.h"



SimpleLight::SimpleLight(SColor lightColor, glm::vec4 lightPosition)
	: ALight(ELightType::SIMPLE, lightColor, lightPosition)
{
}


SimpleLight::~SimpleLight()
{
}
