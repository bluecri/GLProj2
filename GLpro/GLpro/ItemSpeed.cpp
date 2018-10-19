#include "stdafx.h"
#include "ItemSpeed.h"
#include "CollisionComponent.h"
#include "CollisionComponentManager.h"
#include "src/Resource/ModelManager.h"
#include "src/Resource/TextureManager.h"
#include "src/Shader/ShaderManager.h"

#include "IBuff.h"
#include "SpeedBuff.h"

ItemSpeed::ItemSpeed(ENUM_FieldItemForBuff_TYPE fieldItemForBuffType, GameSession * gSession, glm::vec3 & genPos)
	: IFieldItemForBuff(fieldItemForBuffType, gSession,
		GModelManager->getModelWithFileName("data/Model/diamond.obj"),
		GTextureManager->getTextureWithFileName("data/Texture/uvmap.DDS", "dds"),
		GShaderManager->m_addShader<SHADER::ShaderMain>(ENUM_SHADER_TYPE::SHADER_TYPE_MAIN, "data/Shader/ShadowMapping.vertexshader", "data/Shader/ShadowMapping.fragmentshader")
	)
{
	_rigidbodyComponent->setModelMatrix(genPos);

	IFieldItemInit(30.0f, 5.0f, 1);
	initCollisionComponent(GCollisionComponentManager->GetNewAABBCollisionComp(getRigidbodyComponent(), glm::vec3(), glm::vec3(0.5f, 0.5f, 0.5f)));

	setBuff(new SpeedBuff(1.5f));
}

ItemSpeed::~ItemSpeed()
{
}
