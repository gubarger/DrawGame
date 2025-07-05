#include "game_object.h"
#include "../physics/rigidbody.h"
#include "../physics/collision/collider.h"

GameObject::~GameObject() {
    if (physicsBody) delete physicsBody;
    if (objectCollider) delete objectCollider;
}

void GameObject::SetCollider(Collider* newCollider) {
    if (objectCollider != newCollider)
    {
        delete objectCollider;
        objectCollider = newCollider;
    }
}