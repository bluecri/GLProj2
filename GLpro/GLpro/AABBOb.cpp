#include "AABBOb.h"

AABBOb::AABBOb()
{
	_worldVec = glm::vec3();
	_halfAxisSize = glm::vec3();
}

void AABBOb::updateAABBOb(glm::vec3 & center, glm::vec3 & halfAxisSize)
{
	_worldVec = center;
	_halfAxisSize = halfAxisSize;
}

void AABBOb::updateAABBObAxis(glm::vec3 & halfAxisSize)
{
	_halfAxisSize = halfAxisSize;
}

void AABBOb::updateAABBObAxis(float& halfAxisSize)
{
	_worldVec[0] = halfAxisSize;
	_worldVec[1] = halfAxisSize;
	_worldVec[2] = halfAxisSize;
}

void AABBOb::updateAABBObCenter(glm::mat4 & center)
{
	for (int i = 0; i < 3; i++)
	{
		_worldVec[i] = center[3][i];
	}
}

void AABBOb::updateAABBObCenter(glm::vec3 & center)
{
	_worldVec = center;
}

void AABBOb::updateAABBObCenter(glm::vec4 & center)
{
	_worldVec[0] = center[0];
	_worldVec[1] = center[1];
	_worldVec[2] = center[2];
}

bool AABBOb::checkINFLineIntersection(glm::vec3 & lineStartPos, glm::vec3& lineVec, float & resultDist)
{
	float minDist = std::numeric_limits<float>::lowest();
	float maxDist = std::numeric_limits<float>::infinity();
	glm::vec3 T_vec1, T_vec2;

	if (checkPointInAABB(lineStartPos))
	{
		resultDist = 0.0f;
		return true;
	}

	for (int i = 0; i < 3; i++)
	{
		if (lineVec[i] < 0.001f)
		{
			// pararell
			if ((lineStartPos[i] < _worldVec[i] - _halfAxisSize[i]) || (lineStartPos[i] > _worldVec[i] + _halfAxisSize[i]))
				return false;
		}
		else
		{
			T_vec1[i] = (_worldVec[i] - _halfAxisSize[i] - lineStartPos[i]) / lineVec[i];
			T_vec2[i] = (_worldVec[i] + _halfAxisSize[i] - lineStartPos[i]) / lineVec[i];

			if (T_vec1[i] > T_vec2[i])
			{
				float temp = T_vec1[i];
				T_vec1[i] = T_vec2[i];
				T_vec2[i] = temp;
			}

			if (T_vec1[i] > minDist)
			{
				minDist = T_vec1[i];
			}

			if (T_vec2[i] < maxDist)
			{
				maxDist = T_vec2[i];
			}

			if ((minDist > maxDist) || (maxDist < 0))
			{
				return false;
			}
		}
	}

	resultDist = minDist;
	return true;
}

bool AABBOb::checkLineIntersection(glm::vec3 & lineStartPos, glm::vec3 & lineVec, float len, float & resultDist)
{
	glm::vec3 T_vec1;
	int longestIdx;

	if (checkPointInAABB(lineStartPos))
	{
		resultDist = 0.0f;
		return true;
	}


	for (int i = 0; i < 3; i++)
	{
		if (lineVec[i] < 0.001f)
		{
			// pararell
			if ((lineStartPos[i] < _worldVec[i] - _halfAxisSize[i]) || (lineStartPos[i] > _worldVec[i] + _halfAxisSize[i]))
				return false;
		}
		else
		{
			if (lineVec[i] > 0.0f)
			{
				T_vec1[i] = (_worldVec[i] - _halfAxisSize[i] - lineStartPos[i]) / lineVec[i];
			}
			else
			{
				T_vec1[i] = (_worldVec[i] + _halfAxisSize[i] - lineStartPos[i]) / lineVec[i];
			}

			if (resultDist < T_vec1[i])
			{
				resultDist = T_vec1[i];
				longestIdx = i;
			}
		}
	}

	// check dist in range [0 ~ len]
	if (!(0 <= resultDist && resultDist <= len))
	{
		return false;
	}

	// check hit point is in AABB
	int idx1 = (longestIdx + 1) % 3;
	int idx2 = (longestIdx + 2) % 3;
	
	float otherLen1 = lineStartPos[idx1] + lineVec[idx1] * len;
	float otherLen2 = lineStartPos[idx2] + lineVec[idx2] * len;

	if (!(_worldVec[idx1] - _halfAxisSize[idx1] <= otherLen1 && otherLen1 <= _worldVec[idx1] + _halfAxisSize[idx1]))
	{
		return false;
	}

	if (!(_worldVec[idx2] - _halfAxisSize[idx2] <= otherLen2 && otherLen2 <= _worldVec[idx2] + _halfAxisSize[idx2]))
	{
		return false;
	}

	return true;
}

bool AABBOb::checkPointInAABB(glm::vec3 & pos)
{
	for (int i = 0; i < 3; i++)
	{
		if (!(_worldVec[i] - _halfAxisSize[i] <= pos[i] && pos[i] <= _worldVec[i] + _halfAxisSize[i]))
		{
			return false;
		}
	}
	return true;
}


const glm::vec3& AABBOb::getCenterConstRef() const
{
	return _worldVec;
}

glm::vec3 AABBOb::getCenter()
{
	return _worldVec;
}

const glm::vec3 & AABBOb::getAxisConstRef() const
{
	return _halfAxisSize;
}

glm::vec3 AABBOb::getAxis()
{
	return _halfAxisSize;
}
