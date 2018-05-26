#include "stdafx.h"
#include "IMissile.h"

#include "src/RenderTarget/Normal/NormalFObj.h"
#include "RenderManager.h"
#include "src/Render/RNormal.h"
#include "src/Shader/ShaderMain.h"
#include "CollisionComponent.h"

IMissile::IMissile(int type, Entity* fromEntity, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: IPlane(type, model, texture, shadermain), _fromEntity(fromEntity)
{
}

IMissile::~IMissile()
{

}

