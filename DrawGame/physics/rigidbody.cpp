#include "rigidbody.h"
#include <iostream>

void RigidBody::Update(float deltaTime) {
	if (isStatic) return;

	// F = m*a -> a = F/m
	glm::vec3 acceleration = force / mass;
	velocity += acceleration * deltaTime;
	position += velocity * deltaTime;

	force = glm::vec3(0.0f);
}