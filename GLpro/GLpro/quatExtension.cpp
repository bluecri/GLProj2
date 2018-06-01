#include "stdafx.h"
#include "quatExtension.h"

glm::quat RotateTowards(glm::quat q1, const glm::quat & q2, float maxAngle) {

	if (maxAngle < 0.001f) {
		// No rotation allowed. Prevent dividing by 0 later.
		return q1;
	}

	float cosTheta = dot(q1, q2);

	// q1 and q2 are already equal.
	// Force q2 just to be sure
	if (cosTheta > 0.9999f) {
		return q2;
	}

	// Avoid taking the long path around the sphere
	if (cosTheta < 0) {
		q1 = q1*-1.0f;
		cosTheta *= -1.0f;
	}

	float angle = acos(cosTheta);

	// If there is only a 2&deg; difference, and we are allowed 5&deg;,
	// then we arrived.
	if (angle < maxAngle) {
		return q2;
	}

	float fT = maxAngle / angle;
	angle = maxAngle;

	glm::quat res = (sin((1.0f - fT) * angle) * q1 + sin(fT * angle) * q2) / sin(angle);
	res = glm::normalize(res);
	return res;
}
