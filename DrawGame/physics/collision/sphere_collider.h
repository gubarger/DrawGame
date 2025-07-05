#pragma once

#include "collider.h"

class SphereCollider : public Collider {
public:
	SphereCollider(float radius) : radius(radius) {}

	bool CheckCollision(const Collider* other) const override;

	void UpdatePosition(const glm::vec3& position) { 
		center = position; 
	}
	
	glm::vec3 GetCenter() const override { 
		return center; 
	}

	glm::vec3 GetSize() const override {
		return glm::vec3(radius * 2);
	}

	float GetRadius() const { 
		return radius; 
	}

private:
	float radius;
	glm::vec3 center;
};