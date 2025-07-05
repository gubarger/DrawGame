#pragma once

#include <glm/glm.hpp>
#include "../rigidbody.h"

class Collider {
public:
	virtual ~Collider() = default;
	virtual bool CheckCollision(const Collider* other) const = 0;
	virtual void UpdatePosition(const glm::vec3& position) = 0;

	// for all colliders
	virtual glm::vec3 GetCenter() const = 0;
	virtual glm::vec3 GetSize() const = 0;

	void SetRigidBody(RigidBody* body) { 
		rigidBody = body; 
	}

	RigidBody* GetRigidBody() const { 
		return rigidBody; 
	}

protected:
	RigidBody* rigidBody = nullptr;
};