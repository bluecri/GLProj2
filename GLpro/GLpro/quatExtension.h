#pragma once

#include <glm/gtx/transform.hpp>
#include <glm/gtx/quaternion.hpp>

glm::quat RotateTowards(glm::quat q1, const glm::quat& q2, float maxAngle);