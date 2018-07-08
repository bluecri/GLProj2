#include "SpotLight.h"

SpotLight::SpotLight(ILightWithEntityManager * bindedLightManager, GameSession * gSession, int uboType, int idx, glm::mat4 rotMat, glm::vec3 & pos, glm::vec4 & color, float lightPower, float fovy, float aspcect, float persNear, float persFar)
	: LightWithEntity(bindedLightManager, gSession, uboType, idx, rotMat, pos, color, lightPower), _fovy(fovy), _aspcect(aspcect), _persNear(persNear), _persFar(persFar)
{
	_perspectiveMat = glm::perspective(_fovy, _aspcect, _persNear, _persFar);

	lightUpdate();
}

void SpotLight::lightUpdate()
{
	// dir update
	const glm::mat4& lightWorldMat = getWorldMat();
	//_lightDir = lightWorldMat[2];

	// _vpMat update
	glm::vec3 tempEye = lightWorldMat[3];
	glm::vec3 fowardVec = lightWorldMat[2];
	glm::vec3 upVec = lightWorldMat[1];

	_viewMat = glm::lookAt(tempEye, fowardVec + tempEye, upVec);

	_vpMat = _perspectiveMat * _viewMat;

	_frustumOb.updateFrustumOb(_perspectiveMat, _viewMat);
}

glm::mat4& SpotLight::getVPMatRef()
{
	return _vpMat;
}

glm::mat4 & SpotLight::GetViewMat()
{
	return _viewMat;
}

glm::mat4 & SpotLight::GetProjMat()
{
	return _perspectiveMat;
}

FrustumOb & SpotLight::getFrustumObRef()
{
	return _frustumOb;
}
