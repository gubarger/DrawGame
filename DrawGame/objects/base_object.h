#pragma once

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "game_object.h"
#include "../physics/rigidbody.h"
#include "../src/shader.h"

class BaseObject : public GameObject {
public:
    virtual ~BaseObject() override = default;
    virtual void Draw(Shader& shader) = 0;

    Collider* GetCollider() const;

    virtual void SetPosition(const glm::vec3& newPosition) = 0;
    virtual void Update(float deltaTime);

    RigidBody* physicsBody = nullptr;

    static BaseObject* Create(int type);
};