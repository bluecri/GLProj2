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

NormalMissileGenerator::NormalMissileGenerator()
	: IMissileGenerator(new SpecifiedNormalMissileState(), new SpecifiedNormalMissileState())
{
	_missileType = ENUM_MISSILE_TYPE::ENUM_MISSILE_TYPE_NORMAL;
}

void NormalMissileGenerator::genMissile()
{
	if (_curMissileDelay < _curCommonMissileState->_shotDelay)
	{
		return;
	}

	NormalMissile* normalMissile = new NormalMissile(_bindedEntity, _bindedEntity->getGameSession(), _curCommonMissileState);
	normalMissile->initNormalMissile(_entityRigidbodyComponent->getWorldMatRef(), static_cast<SpecifiedNormalMissileState*>(_curSpecifiedMissileState));

	normalMissile->_startSound->play();		// gen sound play
	_curMissileDelay = 0.0f;
}

void NormalMissileGenerator::updateTimer(float deltaTime, float acc)
{
	_curMissileDelay += deltaTime;
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

void NormalMissileGenerator::modifyCurMissileStateWithBuffInfo(BuffInfo * buffInfo)
{
}

