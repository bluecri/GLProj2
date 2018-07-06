#include "PointLight.h"

std::vector<std::pair<glm::vec3, glm::vec3>>	PointLight::_lookAt12Vec;

PointLight::PointLight(ILightWithEntityManager * bindedLightManager, GameSession * gSession, int uboType, int idx, glm::vec3 & pos, glm::vec4 & color, float lightPower, float persNear, float persFar)
	: LightWithEntity(bindedLightManager, gSession, uboType, idx, glm::mat4(), pos, color, lightPower), _persNear(persNear), _persFar(persFar)
{
	_perspectiveMat = glm::perspective(glm::radians(90.0f), 1.0f, _persNear, _persFar);
	_cubeVPMatVec = std::vector<glm::mat4>(6, glm::mat4());

	float orthoSize = _persFar * 2;
	_shadowOrthoMat = glm::ortho(orthoSize, orthoSize, orthoSize, orthoSize);

	lightUpdate();
}

void PointLight::lightUpdate()
{
	glm::vec3 lightPos = getWorldPos();
	for (int i = 0; i < 6; i++)
	{
		_cubeVPMatVec[i] = _perspectiveMat * glm::lookAt(lightPos, lightPos + _lookAt12Vec[i].first, _lookAt12Vec[i].second);
	}

	// todo : update aabb
}

std::vector<glm::mat4>& PointLight::getVPMatCubeVec()
{
	return _cubeVPMatVec;
}

const AABBOb& PointLight::getAABBObRef()
{
	return _aabbOb;
}

void PointLight::PointLightInit()
{
	//right, left, top, bottom, near, far
	_lookAt12Vec.push_back(std::make_pair(glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f,  1.0f,  0.0f)));
	_lookAt12Vec.push_back(std::make_pair(glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
	_lookAt12Vec.push_back(std::make_pair(glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
	_lookAt12Vec.push_back(std::make_pair(glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
	_lookAt12Vec.push_back(std::make_pair(glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
	_lookAt12Vec.push_back(std::make_pair(glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
}
