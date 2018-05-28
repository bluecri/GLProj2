#pragma once
#include "stdafx.h"
#include "CommonMissileState.h"

namespace RESOURCE { class Model;  class Texture; }
namespace SHADER { class ShaderMain; }

class Transform;
class ALSource;
class GameSession;
class Entity;

class MissileGeneratorStorage
{
public:
	MissileGeneratorStorage()
	{
	}

	void addMissileGenerator(IMissileGenerator* missileGenerator);
	void update(float deltaTime, float acc);
	void shotMissile(int missileIndex);
private:
	Entity*			_bindedEntity;
	std::vector<IMissileGenerator*> _missileGeneratorVec;

	
};