#pragma once
#include <glm/ext.hpp> 
#include <vector>
#include "vertex.h"

glm::vec3 computeModelWorldPosition(const std::vector<vertex>& vertices, const glm::mat4& modelMatrix);