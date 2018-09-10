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
	IGameObject(ENUM_ENTITY_TYPE type, GameSession* gSession, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	~IGameObject();

	void					initCollisionComponent(CollisionComponent* collisionComp);

	virtual void			setBRender(bool bRender) override;
	virtual void			setCollisionTest(bool bCollision) override;

	RENDER::RNormal*		_rNormal;		//which rbox get(which shader? use same rbox)

protected:
	void					collisionLogicUpdate();									// collision events 처리
	virtual void			collisionFunc(CollisionComponent* collisionComp) = 0;	// collisionLogicUpdate에서 사용하는 collision 처리 함수
	std::shared_ptr<RENDER::RNormal::DrawElement> getRendererSharedElem();

private:
	std::shared_ptr<RENDER::RNormal::DrawElement> registeredElemInRenderer;
	CollisionComponent*		_collisionComp;
};