#include "base_object.h"
#include "cube.h"
#include "sphere.h"

BaseObject* BaseObject::Create(int type) {
    switch (type) 
    {
    case 1:
        return new Cube();
    case 2:
        return new Sphere();
    default:
        std::cerr << "Unknown object type: " << type << std::endl;
        return nullptr;
    }
}

Collider* BaseObject::GetCollider() const {
    return objectCollider;
}

void BaseObject::SetPosition(const glm::vec3& newPosition) {
    position = newPosition;

    if (physicsBody) 
    {
        physicsBody->position = newPosition;
    }
    if (objectCollider) 
    {
        objectCollider->UpdatePosition(newPosition);
    }
}

void BaseObject::Update(float deltaTime) {
    if (physicsBody) 
    {
        position = physicsBody->position;
    
        if (objectCollider) 
        {
            objectCollider->UpdatePosition(position);
        }
    }
}