#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "./src/Entity.h"
#include "./src/Render/RNormal.h"

//#include "CollisionComponent.h"
class CollisionComponent;

namespace RENDER_TARGET
{
	namespace NORMAL
	{
		class NormalFObj;
	}
}
namespace SHADER { class ShaderMain; }
namespace RESOURCE { class Model;  class Texture;}

class Player : public Entity 
{
public:
	Player(RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	void inputProgress(long long transferKeyInput);
	virtual ~Player();

	virtual void logicUpdate(float deltaTime, float acc) override;
	

	virtual void setBRender(bool bRender);

public:
	CollisionComponent * collisionComp;

	RENDER::RNormal* _rNormal;		//which rbox get(which shader? use same rbox)
	RENDER_TARGET::NORMAL::NormalFObj * _normalFObj;

	float _shotDelay;
	int _shotDmg;
	int _hp;
	int _maxHp;
	int _armor;
	int _maxArmor;

	float _deltaSpeed;

private:
	std::shared_ptr<RENDER::RNormal::DrawElement> registeredElemInRenderer;

};

#endif