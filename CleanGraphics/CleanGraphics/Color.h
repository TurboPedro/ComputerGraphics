#pragma once

#ifndef COLOR_H_
# define COLOR_H_

#include <glm/vec3.hpp>

struct SColor {
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	float Shiness;
};

namespace PremadeColor {
	extern const SColor White;
	extern const SColor Emerald;
	extern const SColor Jade;
	extern const SColor RubberBlack;
	extern const SColor BrickTexture;
}

#endif /* COLOR_H_ */