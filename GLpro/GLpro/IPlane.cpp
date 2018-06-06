#include "IPlane.h"

#include "src/RenderTarget/Normal/NormalFObj.h"
#include "RenderManager.h"
#include "src/Render/RNormal.h"
#include "src/Shader/ShaderMain.h"
#include "CollisionComponent.h"
#include "MissileGeneratorStorage.h"
#include "RigidbodyComponent.h"

IPlane::IPlane(int type, GameSession* gSession, RESOURCE::Model * model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain)
	: Entity(gSession, type)
{
	_rNormal = GRendermanager->getRRender<RENDER::RNormal, SHADER::ShaderMain>(shadermain);
	registeredElemInRenderer = _rNormal->addDrawElem(new RENDER_TARGET::NORMAL::NormalFObj(model, texture), _rigidbodyComponent);

	_maxAngle = 0.01f;
	_angleSpeed = 0.8f;
}

IPlane::~IPlane()
{
	_collisionComp->setDeleted(true);
	registeredElemInRenderer->first->setBDeleted();
}

void IPlane::setBRender(bool bRender)
{
	registeredElemInRenderer->first->setBRender(bRender);
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

void IPlane::setSpeed(float speed)
{
	_rigidbodyComponent->speedSet(speed);
}

void IPlane::setMaxSpeed(float speed)
{
	_rigidbodyComponent->setMaxSpeed(speed);
}

float IPlane::getMaxSpeed()
{
	return _rigidbodyComponent->getMaxSpeed();
}

float IPlane::getSpeed()
{
	return _rigidbodyComponent->getSpeed();
}

float IPlane::getSpeedPerMaxSpeedRatio()
{
	return getSpeed() / getMaxSpeed();
}

void IPlane::collisionLogicUpdate() {
	for (auto elem : _collisionComp->_collisionLogList)
	{
		collisionFunc(elem);
	}

	_collisionComp->_collisionLogList.clear();
}
