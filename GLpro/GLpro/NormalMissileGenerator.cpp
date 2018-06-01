#include "stdafx.h"
#include "NormalMissileGenerator.h"

#include "CommonMissileState.h"
#include "SpecifiedNormalMissileState.h"

#include "CommonNormalMissileInfo.h"
#include "SpecifiedNormalMissileInfo.h"
#include "NormalMissile.h"
#include "./src/Transform.h"
#include "MissileGeneratorStorage.h"
#include "GameSession.h"

#include "src/Sound/ALSource.h"

NormalMissileGenerator::NormalMissileGenerator()
	: IMissileGenerator()
{
	_missileType = ENUM_MISSILE_TYPE::ENUM_MISSILE_TYPE_NORMAL;
}

void NormalMissileGenerator::genMissile()
{
	if (_curMissileDelay < _commonMissileState->_missileDelay)
	{
		return;
	}

	NormalMissile* normalMissile = new NormalMissile(_bindedEntity, _bindedEntity->_gameSession, _commonMissileState);
	normalMissile->init(_entityTransform->getWorldMatRef(), static_cast<SpecifiedNormalMissileState*>(_specifiedMissileState));

	normalMissile->_startSound->play();		// gen sound play
	printf_s("[Log] : missile gened\n");
	_curMissileDelay = 0.0f;
}

void NormalMissileGenerator::updateTimer(float deltaTime, float acc)
{
	_curMissileDelay += deltaTime;
}

void NormalMissileGenerator::init(Entity* bindedEntity, MissileGeneratorStorage* missileGeneratorStorage)
{
	setBindedEntityAndMissileGenerator(bindedEntity, missileGeneratorStorage);

	// init common missile state with common info
	_commonMissileState = new CommonMissileState();

	CommonNormalMissileInfo* commonNormalMissileInfo = new CommonNormalMissileInfo();
	commonNormalMissileInfo->init();		// or filename
	_commonMissileState->init(commonNormalMissileInfo);

	// normal state
	SpecifiedNormalMissileState* tempSpecifiedNormalMissileState = new SpecifiedNormalMissileState();

	SpecifiedNormalMissileInfo* specifiedNormalMissileInfo = new SpecifiedNormalMissileInfo();
	specifiedNormalMissileInfo->init();		// or filename
	tempSpecifiedNormalMissileState->init(specifiedNormalMissileInfo);

	_specifiedMissileState = tempSpecifiedNormalMissileState;
}

void NormalMissileGenerator::init(Entity* bindedEntity, MissileGeneratorStorage* missileGeneratorStorage, std::string & commonMissileInfoFile, std::string & specifiedMissileInfo)
{
	setBindedEntityAndMissileGenerator(bindedEntity, missileGeneratorStorage);

	// common state
	_commonMissileState = new CommonMissileState();

	CommonNormalMissileInfo* commonNormalMissileInfo = new CommonNormalMissileInfo();
	commonNormalMissileInfo->init(commonMissileInfoFile);		// or filename
	_commonMissileState->init(commonNormalMissileInfo);

	// normal state
	SpecifiedNormalMissileState* tempSpecifiedNormalMissileState = new SpecifiedNormalMissileState();
	
	SpecifiedNormalMissileInfo* specifiedNormalMissileInfo = new SpecifiedNormalMissileInfo();
	specifiedNormalMissileInfo->init(specifiedMissileInfo);		// or filename
	tempSpecifiedNormalMissileState->init(specifiedNormalMissileInfo);
	
	_specifiedMissileState = tempSpecifiedNormalMissileState;
}

