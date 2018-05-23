#pragma once

#include "Color.h"
#include <glm\vec4.hpp>

class ALight
{
public:

	enum ELightType {
		SIMPLE,
		MULTIPLE,
		SPOTLIGHT
	};

	ALight(ELightType lightType, SColor lightColor, glm::vec4 lightPosition);
	~ALight();

	ELightType GetType() const;
	virtual SColor GetColor() const;
	virtual void SetColor(SColor lightColor);
	virtual glm::vec4 GetPosition() const;
	virtual void SetPosition(glm::vec4 lightPosition);

protected:

	ELightType type;
	SColor color;
	glm::vec4 position;
};