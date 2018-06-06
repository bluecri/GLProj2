#include "IPlane.h"

#include "src/RenderTarget/Normal/NormalFObj.h"
#include "RenderManager.h"
#include "src/Render/RNormal.h"
#include "src/Shader/ShaderMain.h"
#include "CollisionComponent.h"
#include "MissileGeneratorStorage.h"
#include "RigidbodyComponent.h"
#include "PlaneInfo.h"

IPlane::IPlane(int type, GameSession* gSession, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: IGameObject(type, gSession, model, texture, shadermain)
{
	_curPlaneInfo = nullptr;
	_originPlaneInfo = nullptr;
}

IPlane::~IPlane()
{
	_collisionComp->setDeleted(true);
}

void IPlane::initIPlane(PlaneInfo * planeInfo, int missileStorageNum)
{
	_curPlaneInfo = planeInfo;
	*_originPlaneInfo = *_curPlaneInfo;
	_missileGeneratorStorage = new MissileGeneratorStorage(missileStorageNum, this);
}

bool IPlane::addMissileGenerator(IMissileGenerator * accMissileGenerator)
{
	return _missileGeneratorStorage->addMissileGenerator(accMissileGenerator);
}

PlaneInfo * IPlane::getCurPlaneInfo()
{
	return _curPlaneInfo;
}

PlaneInfo * IPlane::getOriginPlaneInfo()
{
	return _originPlaneInfo;
}

float IPlane::getAngle()
{
	return _curPlaneInfo->_angle;
}

float IPlane::getAngleSpeed() {
	return _curPlaneInfo->_angleSpeed;
}

MissileGeneratorStorage * IPlane::getMissileGeneratorStorage()
{
	return _missileGeneratorStorage;
}

float IPlane::getMaxSpeed()
{
	return _curPlaneInfo->_speed;
}

float IPlane::getSpeed()
{
	return _rigidbodyComponent->getSpeed();
}

float IPlane::getSpeedPerMaxSpeedRatio()
{
	return getSpeed() / getMaxSpeed();
}

float IPlane::getNotDmgedTime()
{
	return _notDmgedTime;
}
