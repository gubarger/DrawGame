#pragma once

#include <vector>
#include "collider.h"
#include "../../src/shader.h"

class PhysicsEngine; // Forward declaration

class CollisionSystem {
public:

	void AddCollider(Collider* collider);
	void RemoveCollider(Collider* collider);
	void CheckAllCollisions(PhysicsEngine& physicsEngine);

	// debugging
	void DrawCollider(Shader& shader, Collider* collider);
	void DrawWireframeCube(Shader& shader);

private:
	std::vector<Collider*> colliders;
};