#pragma once

#include "LightWithEntity.h"
#include "FrustumOb.h"

class SpotLight	: public LightWithEntity
{
public:
	SpotLight(ILightWithEntityManager* bindedLightManager, GameSession* gSession, int uboType, int idx, glm::mat4 rotMat, glm::vec3& pos, glm::vec4& color, float lightPower, float fovy, float aspcect, float persNear, float persFar);

	// LightWithEntity을(를) 통해 상속됨
	virtual void lightUpdate() override;

	glm::mat4&	getVPMatRef();

	glm::mat4&	GetViewMat();
	glm::mat4&	GetProjMat();

	FrustumOb& getFrustumObRef();
	
private:
	glm::mat4	_perspectiveMat;
	glm::mat4	_viewMat;
	FrustumOb	_frustumOb;
	float		_fovy, _aspcect, _persNear, _persFar;

	//result
	glm::mat4	_vpMat;

};