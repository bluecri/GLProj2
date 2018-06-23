#include "LightWithEntity.h"
#include "LightGameObject.h"
#include "ILightWithEntityManager.h"

LightWithEntity::LightWithEntity(ILightWithEntityManager * bindedLightManager, GameSession * gSession, int uboType, int idx, glm::vec3& dir, glm::vec3 & pos, glm::vec4 & color, float lightPower)
	: Light(color, true), _bindedLightManager(bindedLightManager), _idx(idx), _lightPower(lightPower), _lightDir(dir)
{
	_uboType = uboType;
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
		//UBO_INDEX_DEFERRED_POINT_LIGHT
	);

}

void LightWithEntity::lightInit()
{
	_bindedLightGameObject->registerDestroyCallBackFunc(
		[this]() {
		getBindedLightManager()->removeLightWithEntity(shared_from_this());
	}
	);
}


void LightWithEntity::destroyLight()
{
	// delete entity -> callback light to lightManager for delete
	if (_isValid)
	{
		delete _bindedLightGameObject;
	}
}

bool LightWithEntity::isValid()
{
	return _isValid;
}

mat4 & LightWithEntity::getBiasMat()
{
	return _biasMatrix;
}

vec3 & LightWithEntity::getWorldPos()
{
	if (_bindedLightGameObject != nullptr)
		return _bindedLightGameObject->getRigidbodyComponent()->getWorldPosVec();
	return glm::vec3();
}

float LightWithEntity::getLightPower()
{
	return _lightPower;
}

int LightWithEntity::getUboType()
{
	return _uboType;
}

glm::vec3 LightWithEntity::getLightDir()
{
	return _lightDir;
}

ILightWithEntityManager * LightWithEntity::getBindedLightManager()
{
	return _bindedLightManager;
}