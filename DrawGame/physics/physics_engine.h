#pragma once

#include <vector>
#include "rigidbody.h"

class CollisionSystem; // Forward declaration

class PhysicsEngine {
public:
    void AddRigidBody(RigidBody* body);
    void Update(float deltaTime, CollisionSystem& collisionSystem);
    void HandleCollision(RigidBody* bodyA, RigidBody* bodyB);

private:
    std::vector<RigidBody*> bodies;
    glm::vec3 gravity = glm::vec3(0.0f, -9.81f, 0.0f);
};

extern PhysicsEngine physics;