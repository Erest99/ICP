#include "collision.h"


glm::vec3 computeModelWorldPosition(const std::vector<vertex>& vertices, const glm::mat4& modelMatrix) 
{
    glm::vec3 center(0.0f, 0.0f, 0.0f);

    // Compute center of the model in local space
    for (const auto& vertex : vertices) {
        center += vertex.position;
    }

    center /= static_cast<float>(vertices.size());

    // Transform the center position into world space using the model matrix
    glm::vec4 centerWorld = modelMatrix * glm::vec4(center, 1.0f);

    // Convert to glm::vec3 and return
    return glm::vec3(centerWorld);
}