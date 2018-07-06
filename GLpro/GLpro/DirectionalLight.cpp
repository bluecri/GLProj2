#include "DirectionalLight.h"
#include "DirectionalLightManager.h"

DirectionalLight::~DirectionalLight()
{
	_bindedLightManager->removeDirectionalLightWithNoDelete(this);
	delete this;
}

void DirectionalLight::lightUpdate()
{
	// todo : update pos according to camera view

	_frustumOb.updateFrustumOb(_projectionMatrix, _viewMatrix);
}

const FrustumOb & DirectionalLight::getFrustumObRef()
{
	return _frustumOb;
}

DirectionalLight::DirectionalLight(DirectionalLightManager * bindedLightManager, int idx, glm::vec4 & color, const glm::vec3 & lightLookVec3)
	: Light(color, true), _bindedLightManager(bindedLightManager), _idx(idx), _lightLookVec3(lightLookVec3)
{
	//_depthModelMatrix = glm::mat4(1.0);
	_projectionMatrix = ortho<float>(-50, 50, -50, 50, -100, 100);
	_viewMatrix = lookAt(lightLookVec3, vec3(0, 0, 0), vec3(0, 1, 0));
	
	// create proxy matrix
	//_modelMatrix = _projectionMatrix * _viewMatrix;
}
