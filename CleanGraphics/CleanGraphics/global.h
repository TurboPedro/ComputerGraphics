#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

namespace global {
	extern glm::vec3 lightPosition;
	extern float Linear;
	extern float Quadratic;
	extern bool useHDR;
	extern float gammaCorrection;
};