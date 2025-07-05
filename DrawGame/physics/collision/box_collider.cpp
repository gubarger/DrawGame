#include "box_collider.h"
#include "sphere_collider.h"
#include <iostream>

bool BoxCollider::CheckCollision(const Collider* other) const {
    //if (!other) return false;

    if (const auto* box = dynamic_cast<const BoxCollider*>(other)) 
    {
        // AABB vs AABB
        glm::vec3 otherCenter = box->GetCenter();
        glm::vec3 otherSize = box->GetSize();

        return (abs(center.x - otherCenter.x) * 2 < (size.x + otherSize.x) &&
                (abs(center.y - otherCenter.y) * 2 < (size.y + otherSize.y) &&
                (abs(center.z - otherCenter.z) * 2 < (size.z + otherSize.z))));
    }
    else if (const auto* sphere = dynamic_cast<const SphereCollider*>(other))
    {
        // AABB vs Sphere
        glm::vec3 sphereCenter = sphere->GetCenter();
        float sphereRadius = sphere->GetRadius();

        glm::vec3 closest = glm::clamp(sphereCenter, center - size, center + size);
        float distance = glm::distance(sphereCenter, closest);

        return distance < sphereRadius;
    }
    return false;
}