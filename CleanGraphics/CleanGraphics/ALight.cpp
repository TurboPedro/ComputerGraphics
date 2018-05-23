#include "ALight.h"



ALight::ALight(ALight::ELightType lightType, SColor lightColor, glm::vec4 lightPosition)
{
	type = lightType;
	color = lightColor;
	position = lightPosition;
}


ALight::~ALight()
{
}

ALight::ELightType ALight::GetType() const {
	return type;
}

SColor ALight::GetColor() const
{
	return color;
}

void ALight::SetColor(SColor lightColor)
{
	color = lightColor;
}

glm::vec4 ALight::GetPosition() const
{
	return position;
}

void ALight::SetPosition(glm::vec4 lightPosition)
{
	position = lightPosition;
}
