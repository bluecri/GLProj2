#pragma once

#include "IGameObject.h"
#include "./PlaneInfo.h"

class MissileGeneratorStorage;
class IMissileGenerator;
class IBuff;

class IPlane : public IGameObject
{
public:
	IPlane(int type, GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~IPlane();
	void initIPlane(PlaneInfo * planeInfo, int missileStorageNum = 4);

	MissileGeneratorStorage* getMissileGeneratorStorage();

	bool		addMissileGenerator(IMissileGenerator* accMissileGenerator);

	PlaneInfo* getCurPlaneInfo();
	PlaneInfo* getOriginPlaneInfo();

	float		getAngle();
	float		getAngleSpeed();
	float		getMaxSpeed();
	float		getSpeed();
	float		getSpeedPerMaxSpeedRatio();
	float		getNotDmgedTime();

protected:
	PlaneInfo*					_curPlaneInfo;
	PlaneInfo*					_originPlaneInfo;
	MissileGeneratorStorage*	_missileGeneratorStorage;

	std::list<IBuff*>			_buffList;

	// todo : bufflist를 순회하며 buffInfo 생성. buffInfo + cur/origin -> new curInfo 적용.
	float	_notDmgedTime;
};