#pragma once
#include "stdafx.h"
#include "CommonMissileState.h"

namespace RESOURCE { class Model;  class Texture; }
namespace SHADER { class ShaderMain; }

class Transform;
class ALSource;
class GameSession;
class Entity;

enum ENUM_MISSILE_GEN_MATERIAL
{
	ENUM_MISSILE_GEN_MATERIAL_NORMAL,
	ENUM_MISSILE_GEN_MATERIAL_NUM
};

class MissileGenerator
{
public:
	MissileGenerator()
	{
		for (int i = 0; i < ENUM_MISSILE_GEN_MATERIAL_NUM; i++)
		{
			_missileCommonState.push_back(CommonMissileState());
		}
	}

	void init();
	void update(float deltaTime, float acc);
	void shotMissile(ENUM_MISSILE_GEN_MATERIAL missileType);
public:
	Entity*			_bindedEntity;
	GameSession*	_bindedGameSession;

public:
	std::vector<CommonMissileState> _missileCommonState;

	
};