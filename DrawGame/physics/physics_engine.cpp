#include "physics_engine.h"
#include "collision/collision_system.h"

PhysicsEngine physics;

void PhysicsEngine::AddRigidBody(RigidBody* body) {
    bodies.push_back(body);
}

void PhysicsEngine::Update(float deltaTime, CollisionSystem& collisionSystem) {
    for (auto* body : bodies)
    {
        if (!body) continue;

        body->prevPosition = body->position;

        if (!body->isStatic) 
        {
            body->force += gravity * body->mass;
            body->Update(deltaTime);

            if (body->collider) 
            {
                body->collider->UpdatePosition(body->position);
            }
        }
    }

    collisionSystem.CheckAllCollisions(physics);

    for (auto* body : bodies)
    {
        if (!body) continue;
    
        if (body->isColliding)
        {
            body->position = body->prevPosition;
            body->isColliding = false;
        }
    }
}

void PhysicsEngine::HandleCollision(RigidBody* bodyA, RigidBody* bodyB) {
    RigidBody* dynamicBody = bodyA->isStatic ? bodyB : bodyA;
    RigidBody* staticBody = bodyA->isStatic ? bodyA : bodyB;

    glm::vec3 collisionNormal = glm::normalize(dynamicBody->position - staticBody->position);

    float velocityAlongNormal = glm::dot(dynamicBody->velocity, collisionNormal);

    if (velocityAlongNormal < 0) 
    {
        float restitution = (dynamicBody->restitution + staticBody->restitution) / 2.0f;
        float j = -(1.0f + restitution) * velocityAlongNormal;

        dynamicBody->velocity += j * collisionNormal;
    }

    dynamicBody->isColliding = true;
}