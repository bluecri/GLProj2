#include "stdafx.h"
#include "Player.h"
#include "src/Resource/Model.h"
#include "src/Resource/Texture.h"
#include "src/Render/RNormal.h"
#include "src/RenderTarget/Normal/NormalFObj.h"
#include "src/Shader/ShaderMain.h"
#include "RenderManager.h"
#include "CollisionComponent.h"


Player::Player(RESOURCE::Model* model, RESOURCE::Texture * texture, SHADER::ShaderMain * shadermain) : Entity(0)
{
	_rNormal = GRendermanager->getRRender<RENDER::RNormal, SHADER::ShaderMain>(shadermain);

	_normalFObj = new RENDER_TARGET::NORMAL::NormalFObj(model, texture);
	registeredElemInRenderer = _rNormal->addDrawElem(_normalFObj, _rigidbodyComponent);
}

Player::~Player()
{
	_rNormal->removeDrawElem(registeredElemInRenderer);
	delete _normalFObj;
}


void Player::update(float deltaTime)
{
	//collisionComp->
}
