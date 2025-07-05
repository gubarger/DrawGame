#pragma once

#include "base_object.h"
#include <vector>

class Sphere : public BaseObject {
public:
	Sphere();
	//~Sphere() override;

	virtual void Draw(Shader& shader);
	virtual void SetPosition(const glm::vec3& position);

private:
	static std::vector<float> GenerateSphereVertices(float radius, int sectors, int stacks);

	unsigned int VAO, VBO;
	unsigned int vertexCount;

	glm::mat4 model;
	glm::vec3 position;
};