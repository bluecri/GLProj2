#include "stdafx.h"
#include "MissileGenerator.h"

#include "./src/Resource/Model.h"
#include "./src/Resource/ModelManager.h"
#include "./src/Resource/Texture.h"
#include "./src/Resource/TextureManager.h"
#include "./src/Shader/ShaderManager.h"
#include "./src/Shader/ShaderMain.h"

#include "./NormalMissile.h"
#include "./GameSession.h"
#include "./src/Entity.h"

#include "./NormalMissileInfo.h"

void MissileGenerator::init()
{
	_missileModelVec.push_back(nullptr);
	_missileTextureVec.push_back(nullptr);
	_missileShaderMain.push_back(nullptr);
	_curMissileDelaVec.push_back(0.0f);
	_missileDelayVec.push_back(0.0f);

	// normal missile
	NormalMissileInfo nmInfo;

	_missileModelVec[ENUM_MISSILE_GEN_MATERIAL_NORMAL]		= GModelManager->getModelWithFileName("data/Model/missile.obj");
	_missileTextureVec[ENUM_MISSILE_GEN_MATERIAL_NORMAL]	= GTextureManager->getTextureWithFileName("data/Texture/block.dds", "dds");
	_missileShaderMain[ENUM_MISSILE_GEN_MATERIAL_NORMAL]	= GShaderManager->m_addShader<SHADER::ShaderMain>(ENUM_SHADER_TYPE::SHADER_TYPE_MAIN, "data/Shader/ShadowMapping.vertexshader", "data/Shader/ShadowMapping.fragmentshader");
	_curMissileDelaVec[ENUM_MISSILE_GEN_MATERIAL_NORMAL]	= 0.0f;
	_missileDelayVec[ENUM_MISSILE_GEN_MATERIAL_NORMAL]		= 0.5f;
}

void MissileGenerator::update(float deltaTime, float acc)
{
}

void MissileGenerator::shotMissile(ENUM_MISSILE_GEN_MATERIAL missileType)
{
	switch (missileType)
	{
	case ENUM_MISSILE_GEN_MATERIAL::ENUM_MISSILE_GEN_MATERIAL_NORMAL :
		NormalMissile* normalMissile;
		normalMissile = new NormalMissile(_bindedEntity, _missileModelVec[ENUM_MISSILE_GEN_MATERIAL_NORMAL], _missileTextureVec[ENUM_MISSILE_GEN_MATERIAL_NORMAL], _missileShaderMain[ENUM_MISSILE_GEN_MATERIAL_NORMAL]);
		normalMissile->init(_bindedEntity->_rigidbodyComponent->_transform, );
		_bindedGameSession->registerEntityToGameSession(normalMissile);
		break;
	}
}
