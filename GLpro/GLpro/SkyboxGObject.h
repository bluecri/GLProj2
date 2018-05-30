#pragma once

#include "stdafx.h"
#include "src/Entity.h"
#include "src/Render/RSkybox.h"

namespace SHADER { class ShaderSkybox; }
namespace RENDER_TARGET { namespace SKYBOX { class SkyboxFObj; } }
class GameSession;

class SkyboxGObject : public Entity
{
public:
	SkyboxGObject(GameSession* gameSession, SHADER::ShaderSkybox* shader, RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxFObj);
	~SkyboxGObject();

	
	static void preMade();

public:
	RENDER::RSkybox* rSkybox;
	RENDER_TARGET::SKYBOX::SkyboxFObj* _skyboxFObj;
	static std::vector<SkyboxGObject*> _preMadeSpaceSkybox;

	virtual void setBRender(bool bRender);
private:
	std::shared_ptr<RENDER::RSkybox::DrawElement> registeredElemInRenderer;

	// Entity��(��) ���� ��ӵ�
	virtual void logicUpdate(float deltaTime, float acc) {};	// todo? : div ���⼭ ó��?
	virtual void setCollisionTest(bool bCollision) override;
	virtual void doJobWithBeDeleted() override;
};

