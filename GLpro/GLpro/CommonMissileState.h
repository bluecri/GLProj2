#pragma once

#include "stdafx.h"

namespace RESOURCE { class Model;  class Texture; }
namespace SHADER { class ShaderMain; }

class Transform;
class ALSource;
class GameSession;
class Entity;
class CommonNormalMissileInfo;

class CommonMissileState
{
public:
	CommonMissileState();
	void init(CommonNormalMissileInfo* commonNormalMissileInfo);

	RESOURCE::Model* _missileModel;
	RESOURCE::Texture* _missileTexture;
	SHADER::ShaderMain* _missileShaderMain;

	glm::mat4 collisionBoxMat;
	glm::vec3 missileCollisionBoxAxis;

	float _missileDelay;
};

/*
class CommonMissileState
{
public:
	CommonMissileState()
	{
		_missileModelVec.push_back(nullptr);
		_missileTextureVec.push_back(nullptr);
		_missileShaderMain.push_back(nullptr);
		_missileAddPosVec.push_back(glm::vec3());
		_missileShotSoundStrVec.push_back("");
		_missileHitSoundStrVec.push_back("");
		_curMissileDelaVec.push_back(0.0f);
		_missileDelayVec =0.0f);
	}

	std::vector<RESOURCE::Model*> _missileModelVec;
	std::vector<RESOURCE::Texture*> _missileTextureVec;
	std::vector<SHADER::ShaderMain*> _missileShaderMain;
	std::vector<glm::vec3> _missileAddPosVec;
	std::vector<std::string> _missileShotSoundStrVec;
	std::vector<std::string> _missileHitSoundStrVec;
	std::vector<float> _curMissileDelaVec;
	std::vector<float> _missileDelayVec;
};
*/