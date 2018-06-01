#pragma once

#include "Color.h"
#include <glm\vec4.hpp>

struct SAttenuation {
	float constant;
	float linear;
	float quadratic;
};

namespace PremadeAttenuation {
	extern const SAttenuation d7;
	extern const SAttenuation d13;
	extern const SAttenuation d20;
	extern const SAttenuation d32;
	extern const SAttenuation d50;
	extern const SAttenuation d65;
	extern const SAttenuation d100;
	extern const SAttenuation d160;
	extern const SAttenuation d200;
	extern const SAttenuation d325;
	extern const SAttenuation d600;
	extern const SAttenuation d3250;
	extern const SAttenuation NoAttenuation;
}

class ALight
{
public:

	enum ELightType {
		SIMPLE,
		MULTIPLE,
		SPOTLIGHT
	};

	ALight(ELightType lightType, SColor lightColor, SAttenuation att, glm::vec4 lightPosition);
	~ALight();

	ELightType GetType() const;
	virtual SColor GetColor() const;
	virtual void SetColor(SColor lightColor);
	virtual glm::vec4 GetPosition() const;
	virtual void SetPosition(glm::vec4 lightPosition);
	virtual SAttenuation GetAttenuation() const;
	virtual void SetAttenuation(SAttenuation att);

protected:

	ELightType type;
	SColor color;
	SAttenuation attenuation;
	glm::vec4 position;
};