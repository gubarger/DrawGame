#include "sphere_collider.h"
#include "box_collider.h"
#include <iostream>

bool SphereCollider::CheckCollision(const Collider* other) const {
    if (!other) return false;

    if (const auto* sphere = dynamic_cast<const SphereCollider*>(other))
    {
        // Sphere vs Sphere
        float distance = glm::distance(center, sphere->center);
        return distance < (radius + sphere->radius);
    }
    else if (const auto* box = dynamic_cast<const BoxCollider*>(other)) 
    {
        // Sphere vs AABB
        return box->CheckCollision(this);
    }
    return false;
}