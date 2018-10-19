#include "IPlane.h"

#include "RenderManager.h"
#include "src/Render/RNormal.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "src/Shader/ShaderMain.h"

#include "RigidbodyComponent.h"
#include "CollisionComponent.h"

#include "GameSession.h"
#include "PlaneInfo.h"
#include "MissileGeneratorStorage.h"

#include "IBuff.h"
#include "BuffSum.h"
#include "BuffManager.h"

IPlane::IPlane(ENUM_ENTITY_TYPE type, GameSession* gSession, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: IGameObject(type, gSession, model, texture, shadermain)
{
	_curPlaneInfo = nullptr;
	_originPlaneInfo = nullptr;
	_buffManager = new BuffManager();
	_timeStampNotDmgedTime = -100.0f;
}

IPlane::~IPlane()
{
}

void IPlane::initIPlane(PlaneInfo * curPlaneInfo, PlaneInfo * originPlaneInfo, int missileStorageNum)
{
	_curPlaneInfo = curPlaneInfo;
	_originPlaneInfo = originPlaneInfo;
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
	return _originPlaneInfo->_maxSpeed;
}

float IPlane::getSpeed()
{
	return _rigidbodyComponent->getSpeed();
}

float IPlane::getSpeedPerMaxSpeedRatio()
{
	return getSpeed() / getMaxSpeed();
}

bool IPlane::isCanBeDamaged()
{
	if (_gameSession->getGSessionStartTimeStamp() - _timeStampNotDmgedTime > _curPlaneInfo->_notDmgedTime)
		return true;
	return false;
}

void IPlane::notDamagedTimeStampWrite()
{
	_timeStampNotDmgedTime = _gameSession->getGSessionStartTimeStamp();
}

void IPlane::planeDamaged(int dmg, bool turnOnNotDamaged)
{
	if (turnOnNotDamaged)
	{
		notDamagedTimeStampWrite();
	}

	if (_curPlaneInfo->_armor < dmg)
	{
		_curPlaneInfo->_hp -= dmg - _curPlaneInfo->_armor;
		_curPlaneInfo->_armor = 0;
	}
	else
	{
		_curPlaneInfo->_armor -= dmg;
	}
}

BuffManager * IPlane::getBuffManager()
{
	return _buffManager;
}

void IPlane::transferBuffSum(BuffSum* buffSum)
{
	// adjust buffSum
	_bInvisible = buffSum->bOnce[ENUM_BUFFSUM_ONCE_PLANE_INVISIBLE];
	_bOverwhelming = buffSum->bOnce[ENUM_BUFFSUM_ONCE_PLANE_OVERWHELMING];
	_bRandomDirection = buffSum->bOnce[ENUM_BUFFSUM_ONCE_PLANE_RANDOM_DIRECTION];

	_curPlaneInfo->transferBuffSum(buffSum, _originPlaneInfo);
	_missileGeneratorStorage->transferBuffSum(buffSum);
}
