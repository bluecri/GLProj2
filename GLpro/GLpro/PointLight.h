#pragma once

#include "LightWithEntity.h"

class PointLight : public LightWithEntity
{
public:
	PointLight(ILightWithEntityManager* bindedLightManager, GameSession* gSession, int uboType, int idx, glm::vec3& pos, glm::vec4& color, float lightPower, float persNear = 0.1f, float persFar = 2.0f);

	// LightWithEntity을(를) 통해 상속됨
	virtual void lightUpdate() override;

	std::vector<glm::mat4>&		getVPMatCubeVec();

	static void PointLightInit();
protected:
	glm::mat4				_perspectiveMat;
	std::vector<glm::mat4>	_cubeVPMatVec;

	float	_persNear;
	float	_persFar;
private:
	static std::vector<std::pair<glm::vec3, glm::vec3>>	_lookAt12Vec;

};