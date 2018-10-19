#include "stdafx.h"
#include "IMissile.h"

#include "src/RenderTarget/Normal/NormalFObj.h"
#include "RenderManager.h"
#include "src/Render/RNormal.h"
#include "src/Shader/ShaderMain.h"
#include "CollisionComponent.h"


IMissile::IMissile(ENUM_ENTITY_TYPE type, GameSession * gSession, Entity * fromEntity, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: IGameObject(type, gSession, model, texture, shadermain), _fromEntity(fromEntity)
{
}

IMissile::~IMissile()
{

}
