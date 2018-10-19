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
	IPlane(ENUM_ENTITY_TYPE type, GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
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

	void			planeDamaged(int dmg, bool turnOnNotDamaged);

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