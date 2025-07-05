#pragma once

#include "collider.h"

class BoxCollider : public Collider {
public:
	BoxCollider(const glm::vec3& size) : size(size) {}

	bool CheckCollision(const Collider* other) const override;
	
	void UpdatePosition(const glm::vec3& position) { 
		center = position; 
	}

	glm::vec3 GetCenter() const override { 
		return center; 
	}

	glm::vec3 GetSize() const {
		return size;
	}

private:
	glm::vec3 size;
	glm::vec3 center;
};