#include "collision_system.h"
#include <iostream>
#include "box_collider.h"
#include "sphere_collider.h"
#include <glm/ext/matrix_transform.hpp>
#include "../rigidbody.h"
#include "../physics_engine.h"

void CollisionSystem::AddCollider(Collider* collider) {
    colliders.push_back(collider);
}

void CollisionSystem::RemoveCollider(Collider* collider) {
    colliders.erase(std::remove(colliders.begin(), colliders.end(), collider), colliders.end());
}

void CollisionSystem::CheckAllCollisions(PhysicsEngine& physicsEngine) {
    for (size_t i = 0; i < colliders.size(); ++i) 
    {
        for (size_t j = i + 1; j < colliders.size(); ++j) 
        {
            if (colliders[i] && colliders[j] && colliders[i]->CheckCollision(colliders[j])) 
            {
                RigidBody* bodyA = colliders[i]->GetRigidBody();
                RigidBody* bodyB = colliders[j]->GetRigidBody();

                if (bodyA || bodyB) 
                {
                    physicsEngine.HandleCollision(bodyA ? bodyA : bodyB, bodyB ? bodyB : bodyA);
                }
            }
        }
    }
}

void CollisionSystem::DrawCollider(Shader& shader, Collider* collider) {
    if (auto box = dynamic_cast<BoxCollider*>(collider))
    {
        glm::mat4 model = glm::translate(glm::mat4(1.0f), box->GetCenter()) * glm::scale(glm::mat4(1.0f), box->GetSize() * 2.0f);
        
        shader.setMat4("model", model);
        
        DrawWireframeCube(shader);
    }
}

void CollisionSystem::DrawWireframeCube(Shader& shader) {
    static bool initialized = false;
    static unsigned int VAO, VBO, EBO;

    if (!initialized) 
    {
        float vertices[] = {
            -0.5f, -0.5f, -0.5f,
             0.5f, -0.5f, -0.5f,
             0.5f,  0.5f, -0.5f,
            -0.5f,  0.5f, -0.5f,
            -0.5f, -0.5f,  0.5f,
             0.5f, -0.5f,  0.5f,
             0.5f,  0.5f,  0.5f,
            -0.5f,  0.5f,  0.5f
        };

        unsigned int indices[] = 
        {
            0, 1, 1, 2, 2, 3, 3, 0,
            4, 5, 5, 6, 6, 7, 7, 4,
            0, 4, 1, 5, 2, 6, 3, 7
        };

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        initialized = true;
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}