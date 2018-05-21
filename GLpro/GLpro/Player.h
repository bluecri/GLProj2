#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "./src/Entity.h"

//#include "CollisionComponent.h"
class CollisionComponent;


namespace RENDER_TARGET
{
	namespace NORMAL
	{
		class NormalFObj;
	}
}
namespace RENDER { class RNormal; }
namespace SHADER { class ShaderMain; }
namespace RESOURCE { class Model;  class Texture;}

class Player : public Entity 
{
public:
	Player(RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain);
	virtual ~Player();

	void update(float deltaTime);
		

public:
	CollisionComponent * collisionComp;
	//CONTROL::InputManager * _inputManagerPtr = nullptr;

	RENDER::RNormal* _rNormal;		//which rbox get(which shader? use same rbox)
	RENDER_TARGET::NORMAL::NormalFObj * _normalFObj;
private:
	std::shared_ptr<RNormal::DrawElement> registeredElemInRenderer;

};

#endif