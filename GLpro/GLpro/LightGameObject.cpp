#include "LightGameObject.h"

LightGameObject::LightGameObject(GameSession * gSession) : IGameObject(ENUM_ENTITY_LIGHT, gSession, nullptr, nullptr, nullptr)
{
}

void LightGameObject::doJobWithBeDeleted()
{
}

void LightGameObject::logicUpdate(float deltaTime, float acc)
{
}

void LightGameObject::collisionFunc(CollisionComponent * collisionComp)
{
}
