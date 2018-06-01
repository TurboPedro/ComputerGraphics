#include "SimpleLight.h"

SimpleLight::SimpleLight(SColor lightColor, SAttenuation att, glm::vec4 lightPosition)
	: ALight(ELightType::SIMPLE, lightColor, att, lightPosition)
{
}


SimpleLight::~SimpleLight()
{
}
