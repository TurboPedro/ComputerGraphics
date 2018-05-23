#pragma once

#include <glm\vec2.hpp>
#include <glm\vec3.hpp>

struct SVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	glm::vec3 Tangent;
	glm::vec3 BitTangent;
};