#include "stdafx.h"
#include "IPlane.h"

#include "src/RenderTarget/Normal/NormalFObj.h"
#include "RenderManager.h"
#include "src/Render/RNormal.h"
#include "src/Shader/ShaderMain.h"
#include "CollisionComponent.h"

IPlane::IPlane(int type, GameSession* gSession, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: Entity(gSession, type)
{
	_rNormal = GRendermanager->getRRender<RENDER::RNormal, SHADER::ShaderMain>(shadermain);

	_normalFObj = new RENDER_TARGET::NORMAL::NormalFObj(model, texture);
	registeredElemInRenderer = _rNormal->addDrawElem(_normalFObj, _rigidbodyComponent);
}

IPlane::~IPlane()
{
	_rNormal->removeDrawElem(registeredElemInRenderer);
	delete _normalFObj;
	_collisionComp->setDeleted(true);
}

void IPlane::setBRender(bool bRender)
{
	_normalFObj->setBRender(bRender);
}

void IPlane::setCollisionTest(bool bCollision)
{
	if(_collisionComp != nullptr)
		_collisionComp->setCollisionTest(bCollision);
}


void IPlane::collisionLogicUpdate() {
	for (auto elem : _collisionComp->_collisionLogList)
	{
		collisionFunc(elem);
	}

	_collisionComp->_collisionLogList.clear();
}
