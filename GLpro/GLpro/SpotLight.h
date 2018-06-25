#pragma once

#include "LightWithEntity.h"

class SpotLight	: public LightWithEntity
{
public:
	SpotLight(ILightWithEntityManager* bindedLightManager, GameSession* gSession, int uboType, int idx, glm::mat4 rotMat, glm::vec3& pos, glm::vec4& color, float lightPower, float fovy, float aspcect, float persNear, float persFar);
	void		updateLIght();
	glm::mat4&	getVPMatRef();

	glm::mat4&	GetViewMat();
	glm::mat4&	GetProjMat();
	
private:
	glm::mat4	_perspectiveMat;
	glm::mat4	_viewMat;
	float		_fovy, _aspcect, _persNear, _persFar;

	//result
	glm::mat4	_vpMat;
};