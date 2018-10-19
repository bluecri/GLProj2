#pragma once

#include "LightWithEntity.h"
#include "AABBOb.h"

class PointLight : public LightWithEntity
{
public:
	PointLight(ILightWithEntityManager* bindedLightManager, GameSession* gSession, int uboType, int idx, glm::vec3& pos, glm::vec4& color, float lightPower, float persNear = 0.1f, float persFar = 8.0f);

	// LightWithEntity을(를) 통해 상속됨
	virtual void lightUpdate() override;

	std::vector<glm::mat4>&		getVPMatCubeVec();
	AABBOb&				getAABBObRef();

	static void PointLightInit();
protected:
	glm::mat4				_perspectiveMat;
	glm::mat4				_shadowOrthoMat;
	std::vector<glm::mat4>	_cubeVPMatVec;

	AABBOb					_aabbOb;

	float	_persNear;
	float	_persFar;
private:
	static std::vector<std::pair<glm::vec3, glm::vec3>>	_lookAt12Vec;

};