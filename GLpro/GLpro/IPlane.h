#pragma once

#include "IGameObject.h"
#include "./PlaneInfo.h"

class MissileGeneratorStorage;
class IMissileGenerator;
class IBuff;
class BuffSum;
class BuffManager;

class IPlane : public IGameObject
{
public:
	IPlane(int type, GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~IPlane();
	void initIPlane(PlaneInfo * curPlaneInfo, PlaneInfo * originPlaneInfo, int missileStorageNum = 4);

	MissileGeneratorStorage* getMissileGeneratorStorage();

	bool			addMissileGenerator(IMissileGenerator* accMissileGenerator);

	PlaneInfo*		getCurPlaneInfo();
	PlaneInfo*		getOriginPlaneInfo();

	float			getAngle();
	float			getAngleSpeed();
	float			getMaxSpeed();
	float			getSpeed();
	float			getSpeedPerMaxSpeedRatio();

	bool			isCanBeDamaged();
	void			notDamagedTimeStampWrite();

	void			planeDamaged(int dmg, bool turnOnNotDamaged)
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

	BuffManager*	getBuffManager();

protected:
	void			transferBuffSum(BuffSum * buffSum);

protected:
	PlaneInfo*					_curPlaneInfo;
	PlaneInfo*					_originPlaneInfo;
	MissileGeneratorStorage*	_missileGeneratorStorage;

	BuffManager*				_buffManager;

	float	_timeStampNotDmgedTime;

	bool	_bInvisible;
	bool	_bOverwhelming;
	bool	_bRandomDirection;
};