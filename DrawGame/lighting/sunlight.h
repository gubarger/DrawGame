#pragma once

#include <glm/glm.hpp>
#include "../src/shader.h"

class SunLight {
public:
	glm::vec3 direction = glm::vec3(-0.5f, -1.0f, -0.5f);
	glm::vec3 color = glm::vec3(1.0f, 1.0f, 0.95f);

	float intensity = 1.0f;

	void ApplyToShader(Shader& shader) const;
};