/*
#include "DeferredPointLight.h"
#include "DeferredPointLightManager.h"
#include "LightGameObject.h"

mat4 & DeferredPointLight::getBiasMat() 
{
	return _biasMatrix; 
}

vec3 & DeferredPointLight::getWorldPos()
{
	if(_bindedLightGameObject != nullptr)
		return _bindedLightGameObject->getRigidbodyComponent()->getWorldPosVec();
	return glm::vec3();
}

float DeferredPointLight::getLightPower()
{
	return _lightPower;
}

bool DeferredPointLight::isValid() {
	return _isValid;
}

DeferredPointLight::DeferredPointLight(DeferredPointLightManager * bindedLightManager, GameSession* gSession, int idx, glm::vec3& pos, glm::vec4 & color, float lightPower)
	: Light(color, true), _bindedLightManager(bindedLightManager), _idx(idx), _lightPower(lightPower)
{
	_isValid = true;
	_bindedLightGameObject = new LightGameObject(gSession);
	RigidbodyComponent* rgComp = _bindedLightGameObject->getRigidbodyComponent();
	rgComp->setModelMatrix(pos);

	//_depthModelMatrix = glm::mat4(1.0);
	_biasMatrix = mat4(
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	);

}

void DeferredPointLight::lightInit()
{
	_bindedLightGameObject->registerDestroyCallBackFunc(
		[this]() {
		this->getBindedLightManager()->removeDeferredPointLight(shared_from_this());
	}
	);
}

DeferredPointLightManager * DeferredPointLight::getBindedLightManager()
{
	return _bindedLightManager;
}

void DeferredPointLight::destroyLight()
{
	// delete entity -> callback light to lightManager for delete
	if (_isValid)
	{
		delete _bindedLightGameObject;
	}
}

*/