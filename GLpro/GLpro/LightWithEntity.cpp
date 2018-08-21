#include "LightWithEntity.h"
#include "LightGameObject.h"
#include "ILightWithEntityManager.h"

LightWithEntity::LightWithEntity(ILightWithEntityManager * bindedLightManager, GameSession * gSession, int uboType, int idx, glm::mat4& rotMat, glm::vec3 & pos, glm::vec4 & color, float lightPower)
	: Light(color, true), _bindedLightManager(bindedLightManager), _idx(idx), _lightPower(lightPower)
{
	_uboType = uboType;
	_isValid = true;
	_bindedLightGameObject = new LightGameObject(gSession);

	RigidbodyComponent* rgComp = _bindedLightGameObject->getRigidbodyComponent();
	rgComp->setModelMatrix(pos);
	rgComp->setQuaternion(glm::toQuat(rotMat));
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

vec3 LightWithEntity::getWorldPos()
{
	if (_bindedLightGameObject != nullptr)
		return _bindedLightGameObject->getRigidbodyComponent()->getWorldPosVec();
	return glm::vec3();
}

glm::mat4 LightWithEntity::getWorldMat()
{
	if (_bindedLightGameObject != nullptr)
		return _bindedLightGameObject->getRigidbodyComponent()->getWorldMatRef();
	return glm::mat4();
}

float LightWithEntity::getLightPower()
{
	return _lightPower;
}

int LightWithEntity::getUboType()
{
	return _uboType;
}

LightGameObject * LightWithEntity::getLightGameObject()
{
	return _bindedLightGameObject;
}

ILightWithEntityManager * LightWithEntity::getBindedLightManager()
{
	return _bindedLightManager;
}

// for frustum

LightWithEntity::std::list<DrawElement*>& LightWithEntity::getFrustumedDrawElementContainerRef()
{
	return _frustumedDrawElementContainer;
}

const LightWithEntity::std::list<DrawElement*>& LightWithEntity::getConstFrustumedDrawElementContainerRef()
{
	return _frustumedDrawElementContainer;
}

void LightWithEntity::clearFrustumedDrawElementContainer()
{
	_frustumedDrawElementContainer.clear();
}
