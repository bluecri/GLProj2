#pragma once

#include "./src/Entity.h"
#include "./src/Render/RNormal.h"

class CollisionComponent;

namespace RENDER_TARGET { namespace NORMAL { class NormalFObj; } }
namespace SHADER { class ShaderMain; }
namespace RESOURCE { class Model;  class Texture; }

class IPlane : public Entity
{
public:
	IPlane(int type, RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~IPlane();

	virtual void setBRender(bool bRender) override;
	virtual void setCollisionTest(bool bCollision) override;

public:
	RENDER::RNormal* _rNormal;		//which rbox get(which shader? use same rbox)
	RENDER_TARGET::NORMAL::NormalFObj * _normalFObj;
	CollisionComponent * _collisionComp;

protected:
	void collisionLogicUpdate();	// collision events 처리
	virtual void collisionFunc(CollisionComponent* collisionComp) = 0;		// collisionLogicUpdate에서 사용하는 collision 처리 함수

private:
	std::shared_ptr<RENDER::RNormal::DrawElement> registeredElemInRenderer;
};