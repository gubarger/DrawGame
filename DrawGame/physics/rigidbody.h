#pragma once
#include <glm/glm.hpp>

class Collider; // Forward declaration

class RigidBody {
public:
    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 force;
    glm::vec3 prevPosition;

    float mass = 1.0f;
    float restitution = 0.5f; // Coefficient of elasticity
    bool isStatic = false;
    bool isColliding = false;

    Collider* collider = nullptr;

    void Update(float deltaTime);
};