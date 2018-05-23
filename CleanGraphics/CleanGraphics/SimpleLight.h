#pragma once
#include "ALight.h"

class SimpleLight :
	public ALight
{

public:
	SimpleLight(SColor lightColor, glm::vec4 lightPosition);
	~SimpleLight();
};

