#include "ground.h"
#include <glad/glad.h>
#include "../physics/collision/box_collider.h"

Ground::Ground() {
    physicsBody = nullptr;

    float vertices[] = {
                           // normals
        -5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f,
         5.0f, 0.0f, -5.0f, 0.0f, 1.0f, 0.0f,
         5.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f,
        -5.0f, 0.0f,  5.0f, 0.0f, 1.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    model = glm::mat4(1.0f);

    // AABB and add RigidBody & BoxCollider
    physicsBody = new RigidBody();
    physicsBody->isStatic = true;
    physicsBody->position = glm::vec3(0.0f);

    collider = new BoxCollider(glm::vec3(50.0f, 0.1f, 50.0f));
    collider->UpdatePosition(glm::vec3(0.0f));

    physicsBody->collider = collider;
    collider->SetRigidBody(physicsBody);
}

Ground::~Ground() {
    delete collider;
    delete physicsBody;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Ground::SetPosition(const glm::vec3& newPosition) {
    position = newPosition;

    model = glm::translate(glm::mat4(1.0f), newPosition);

    if (collider) 
    {
        collider->UpdatePosition(newPosition);
    }

    if (physicsBody) 
    {
        physicsBody->position = newPosition;
    }
}

void Ground::SetCollider(Collider* newCollider) {
    if (collider != newCollider) 
    {
        delete collider;
        collider = newCollider;
    }
}

void Ground::Draw(Shader& shader) {
    shader.setMat4("model", model);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Ground::SetPhysicsBody(RigidBody* body) {
    if (physicsBody != body)
    {
        delete physicsBody;
        physicsBody = body;

        if (physicsBody && collider)
        {
            physicsBody->collider = collider;
            collider->SetRigidBody(physicsBody);
        }
    }
}