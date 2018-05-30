#pragma once
#include "stdafx.h"
#include "CommonMissileState.h"

namespace RESOURCE { class Model;  class Texture; }
namespace SHADER { class ShaderMain; }

class Transform;
class ALSource;
class GameSession;
class Entity;
class IMissileGenerator;
class EmptyMissileGenerator;

class MissileGeneratorStorage
{
public:
	MissileGeneratorStorage(int maxWeaponCount, Entity* bindedEntity);

	bool addMissileGenerator(IMissileGenerator* missileGenerator);
	void update(float deltaTime, float acc);
	void shotMissile();
	void selectMissileIndex(int idx);

private:
	Entity*			_bindedEntity;
	std::vector<IMissileGenerator*> _missileGeneratorVec;

	int _selectedWeaponIndex;
	int _maxWeaponCount;
};