#pragma once

#include "base_object.h"

class Cube : public BaseObject {
public:
	Cube();
	//~Cube() override;

	void Draw(Shader& shader) override;
	void SetPosition(const glm::vec3& position);

private:
	unsigned int VAO, VBO;

	glm::mat4 model;
	glm::vec3 position;
};