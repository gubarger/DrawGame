#pragma once

#include <glm/glm.hpp>
#include "game_object.h"
#include "../physics/rigidbody.h"
#include "../physics/collision/collider.h"

class Shader;

class GameObject {
public:
    glm::vec3 position;
    RigidBody* physicsBody = nullptr;

    virtual ~GameObject();

    virtual Collider* GetCollider() const { 
        return objectCollider; 
    }
    
    virtual void SetCollider(Collider* collider);

protected:
    Collider* objectCollider = nullptr;
};