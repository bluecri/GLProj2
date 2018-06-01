#include "IPlane.h"

#include "src/RenderTarget/Normal/NormalFObj.h"
#include "RenderManager.h"
#include "src/Render/RNormal.h"
#include "src/Shader/ShaderMain.h"
#include "CollisionComponent.h"
#include "MissileGeneratorStorage.h"

IPlane::IPlane(int type, GameSession* gSession, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: Entity(gSession, type)
{
	_rNormal = GRendermanager->getRRender<RENDER::RNormal, SHADER::ShaderMain>(shadermain);
	registeredElemInRenderer = _rNormal->addDrawElem(new RENDER_TARGET::NORMAL::NormalFObj(model, texture), _rigidbodyComponent);

	_maxAngle = 0.08f;
	_angleSpeed = 0.1f;
}

IPlane::~IPlane()
{
	_rNormal->removeDrawElem(registeredElemInRenderer);
	//delete _normalFObj;
	_collisionComp->setDeleted(true);
}

void IPlane::setBRender(bool bRender)
{
	registeredElemInRenderer->first->setBRender(bRender);
	//_normalFObj->setBRender(bRender);
}

void IPlane::setCollisionTest(bool bCollision)
{
	if(_collisionComp != nullptr)
		_collisionComp->setCollisionTest(bCollision);
}

bool IPlane::addMissileGenerator(IMissileGenerator * accMissileGenerator)
{
	return _missileGeneratorStorage->addMissileGenerator(accMissileGenerator);
}

void IPlane::accAngleSpeed(float acc) {
	_angleSpeed += acc;
}

void IPlane::setAngleSpeed(float speed) {
	_angleSpeed = speed;
}

float IPlane::getAngleSpeed() {
	return _angleSpeed;
}

MissileGeneratorStorage * IPlane::getMissileGeneratorStorage()
{
	return _missileGeneratorStorage;
}

void IPlane::collisionLogicUpdate() {
	for (auto elem : _collisionComp->_collisionLogList)
	{
		collisionFunc(elem);
	}

	_collisionComp->_collisionLogList.clear();
}
