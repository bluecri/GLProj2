#pragma once

#include "src/Entity.h"
#include "./src/Render/RNormal.h"

namespace SHADER { class ShaderMain; }
namespace RESOURCE { class Model;  class Texture; }

class RigidbodyComponent;
class CollisionComponent;
class GameSession;

/*
*	IGameObejct is interface that drawable object with collisionComponent
*	has Entity, collision comp, render object, bindedGameSession
*/

class IGameObject : public Entity
{
public:
	IGameObject(int type, GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	~IGameObject();

	virtual void			setBRender(bool bRender) override;
	virtual void			setCollisionTest(bool bCollision) override;
	void					initCollisionComponent(CollisionComponent* collisionComp);

	RENDER::RNormal*		_rNormal;		//which rbox get(which shader? use same rbox)
	CollisionComponent*		_collisionComp;

protected:
	void					collisionLogicUpdate();									// collision events 처리
	virtual void			collisionFunc(CollisionComponent* collisionComp) = 0;	// collisionLogicUpdate에서 사용하는 collision 처리 함수

private:
	std::shared_ptr<RENDER::RNormal::DrawElement> registeredElemInRenderer;
};