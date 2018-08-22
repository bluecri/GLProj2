#include "OBBOb.h"

OBBOb::OBBOb()
{
	_worldMat = glm::mat4();
	_halfAxisSize = glm::vec3();
}

OBBOb::OBBOb(glm::mat4 & worldMat, glm::vec3 & halfAxisSize)
{
	_worldMat = worldMat;
	_halfAxisSize = halfAxisSize;
}

OBBOb::OBBOb(glm::vec3 & pos, glm::vec3 & halfAxisSize)
{
	_worldMat = glm::mat4();
	for (int i = 0; i < 3; i++)
	{
		_worldMat[3][i] = pos[i];
	}
	_halfAxisSize = halfAxisSize;
}

void OBBOb::updateOBBOb(const glm::mat4 & worldMat, const glm::vec3 & halfAxisSize)
{
	_worldMat = worldMat;
	_halfAxisSize = halfAxisSize;
}

void OBBOb::updateOBBObAxis(const glm::vec3 & halfAxisSize)
{
	_halfAxisSize = halfAxisSize;
}

void OBBOb::updateOBBObMat(const glm::mat4 & worldMat)
{
	_worldMat = worldMat;
}

void OBBOb::updateOBBObMatOnlyRotate(const glm::mat4 & rotMat)
{
	for (int i = 0; i < 3; i++)
	{
		for (int k = 0; k < 3; k++)
		{
			_worldMat[i][k] = rotMat[i][k];
		}
	}
}

const glm::mat4 & OBBOb::getMatConstRef() const
{
	return _worldMat;
}

const glm::vec3 & OBBOb::getAxisConstRef() const
{
	return _halfAxisSize;
}
