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
	extern const SColor Obsidian;
	extern const SColor Pearl;
	extern const SColor Ruby;
	extern const SColor Turquoise;
	extern const SColor Brass;
	extern const SColor Bronze;
	extern const SColor Chrome;
	extern const SColor Copper;
	extern const SColor Gold;
	extern const SColor Silver;
	extern const SColor PlasticBlack;
	extern const SColor PlasticCyan;
	extern const SColor PlasticGreen;
	extern const SColor PlasticRed;
	extern const SColor PlasticWhite;
	extern const SColor PlasticYellow;
	extern const SColor RubberBlack;
	extern const SColor RubberCyan;
	extern const SColor RubberGreen;
	extern const SColor RubberRed;
	extern const SColor RubberWhite;
}

#endif /* COLOR_H_ */