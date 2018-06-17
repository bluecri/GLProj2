#include "stdafx.h"
#include "NormalMissileGenerator.h"

#include "CommonMissileState.h"
#include "SpecifiedNormalMissileState.h"

#include "CommonNormalMissileInfo.h"
#include "SpecifiedNormalMissileInfo.h"
#include "NormalMissile.h"
#include "MissileGeneratorStorage.h"
#include "GameSession.h"

#include "src/Sound/ALSource.h"

#include "BuffSum.h"

NormalMissileGenerator::NormalMissileGenerator()
	: IMissileGenerator(new SpecifiedNormalMissileState(), new SpecifiedNormalMissileState())
{
	_missileType = ENUM_MISSILE_TYPE::ENUM_MISSILE_TYPE_NORMAL;
	_bShotDisable = false;
}

void NormalMissileGenerator::genMissile()
{
	if (_timerMissileDelay < _curCommonMissileState->_shotDelay || _bShotDisable)
	{
		return;
	}

	NormalMissile* normalMissile = new NormalMissile(_bindedEntity, _bindedEntity->getGameSession(), _curCommonMissileState);
	normalMissile->initNormalMissile(_entityRigidbodyComponent->getWorldMatRef(), static_cast<SpecifiedNormalMissileState*>(_curSpecifiedMissileState));

	normalMissile->_startSound->play();		// gen sound play
	_timerMissileDelay = 0.0f;
}

void NormalMissileGenerator::updateTimer(float deltaTime, float acc)
{
	_timerMissileDelay += deltaTime;
}

void NormalMissileGenerator::initNormalMissileGenerator(Entity* bindedEntity, MissileGeneratorStorage* missileGeneratorStorage)
{
	initBindedEntityAndMissileGenerator(bindedEntity, missileGeneratorStorage);

	CommonNormalMissileInfo* commonNormalMissileInfo = new CommonNormalMissileInfo();
	commonNormalMissileInfo->initICommonMissileInfo();		// or filename

	SpecifiedNormalMissileInfo* specifiedNormalMissileInfo = new SpecifiedNormalMissileInfo();
	specifiedNormalMissileInfo->initISpecifiedMissileInfo();		// or filename

	initState(commonNormalMissileInfo, specifiedNormalMissileInfo);
}

void NormalMissileGenerator::initNormalMissileGenerator(Entity* bindedEntity, MissileGeneratorStorage* missileGeneratorStorage, std::string & commonMissileInfoFile, std::string & specifiedMissileInfo)
{
	initBindedEntityAndMissileGenerator(bindedEntity, missileGeneratorStorage);

	CommonNormalMissileInfo* commonNormalMissileInfo = new CommonNormalMissileInfo();
	commonNormalMissileInfo->initICommonMissileInfo(commonMissileInfoFile);		// or filename

	SpecifiedNormalMissileInfo* specifiedNormalMissileInfo = new SpecifiedNormalMissileInfo();
	specifiedNormalMissileInfo->initISpecifiedMissileInfo(specifiedMissileInfo);		// or filename

	initState(commonNormalMissileInfo, specifiedNormalMissileInfo);
}

void NormalMissileGenerator::transferBuffSum(BuffSum * buffSum)
{
	_curCommonMissileState->transferBuffSum(buffSum, _originCommonMissileState);
	_curSpecifiedMissileState->transferBuffSum(buffSum, _originSpecifiedMissileState);
	
	_bShotDisable = buffSum->bOnce[ENUM_BUFFSUM_ONCE_SHOT_DISABLE];
}

