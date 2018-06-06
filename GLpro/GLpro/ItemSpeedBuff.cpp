#include "stdafx.h"
#include "ItemSpeedBuff.h"
#include "CollisionComponent.h"
#include "src/Resource/ModelManager.h"
#include "src/Resource/TextureManager.h"
#include "src/Shader/ShaderManager.h"

ItemSpeedBuff::ItemSpeedBuff(GameSession * gSession, glm::vec3& genPos)
	: IFieldItem(ENUM_ENTITY_TYPE::ENUM_ENTITY_ITEM_SPEEDBUFF, gSession,
		GModelManager->getModelWithFileName("data/Model/diamond.obj"),
		GTextureManager->getTextureWithFileName("data/Texture/uvmap.DDS", "dds"),
		GShaderManager->m_addShader<SHADER::ShaderMain>(ENUM_SHADER_TYPE::SHADER_TYPE_MAIN, "data/Shader/ShadowMapping.vertexshader", "data/Shader/ShadowMapping.fragmentshader")
	)
{
	_rigidbodyComponent->setModelMatrix(genPos);
}

void ItemSpeedBuff::itemInit()
{
}

void ItemSpeedBuff::logicUpdate(float deltaTime, float acc)
{
	collisionLogicUpdate();
	moveLogicUpdate(deltaTime);

	if (_buffCount <= 0)
	{
		_bDeleted = true;
		return;
	}
}

void ItemSpeedBuff::collisionFunc(CollisionComponent * collisionComp)
{
	// collision event 처리 memeber 함수
	Entity* entity = collisionComp->_rigidComp->getBindedEntity();
	int entityType = entity->getType();

	_buffCount = 1;
	// missile collision logic은 모두 missile에서.
	switch (entityType)
	{
	case ENUM_ENTITY_PLANE_PLAYER:
		break;
	case ENUM_ENTITY_ENEMY:
		break;
	default:
		// none
		break;
	}
}

void ItemSpeedBuff::doJobWithBeDeleted()
{
}
