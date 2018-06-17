#pragma once
#include "stdafx.h"
#include "CommonMissileState.h"

namespace RESOURCE { class Model;  class Texture; }
namespace SHADER { class ShaderMain; }

class ALSource;
class GameSession;
class Entity;
class IMissileGenerator;
class EmptyMissileGenerator;

class MissileGeneratorStorage
{
public:
	MissileGeneratorStorage(int maxWeaponCount, Entity* bindedEntity);

	bool		addMissileGenerator(IMissileGenerator* missileGenerator);
	void		update(float deltaTime, float acc);
	void		shotMissile();
	void		selectMissileIndex(int idx);
	void		transferBuffSum(BuffSum * buffSum);

private:
	std::vector<IMissileGenerator*> _missileGeneratorVec;

	Entity*		_bindedEntity;
	int			_selectedWeaponIndex;
	int			_maxWeaponCount;

	bool		_bShotDisable;
};