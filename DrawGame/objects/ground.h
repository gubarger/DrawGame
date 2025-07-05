#pragma once

#include <glm/glm.hpp>
#include "../src/shader.h"
#include "../physics/collision/collider.h"
#include "base_object.h"

class Ground : public BaseObject {
public:
    Ground();
    ~Ground() override;

    void Draw(Shader& shader) override;
    void SetPosition(const glm::vec3& newPosition) override;
    void SetCollider(Collider* collider) override;
    
    Collider* GetCollider() const { 
        return collider; 
    }

    RigidBody* GetPhysicsBody() const { 
        return physicsBody; 
    }

    void SetPhysicsBody(RigidBody* body);

private:
    unsigned int VAO, VBO;

    glm::mat4 model;

    Collider* collider = nullptr;
    RigidBody* physicsBody = nullptr;
};