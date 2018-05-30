#include "SkyboxGObject.h"

#include "src/Entity.h"
#include "src/Shader/ShaderManager.h"
#include "RenderManager.h"
#include "src/RenderTarget/Skybox/SkyboxFObj.h"
#include "src/Shader/ShaderSkybox.h"

std::vector<SkyboxGObject*> SkyboxGObject::_preMadeSpaceSkybox;

SkyboxGObject::SkyboxGObject(GameSession* gameSession, SHADER::ShaderSkybox * shader, RENDER_TARGET::SKYBOX::SkyboxFObj * skyboxFObj)
	:Entity(gameSession)
{
	_skyboxFObj = skyboxFObj;
	rSkybox = GRendermanager->getRRender<RENDER::RSkybox, SHADER::ShaderSkybox>(shader);
	registeredElemInRenderer = rSkybox->addToDrawList(skyboxFObj, _rigidbodyComponent);
}

SkyboxGObject::~SkyboxGObject()
{
	rSkybox->destructor(registeredElemInRenderer);
	delete _skyboxFObj;
}

void SkyboxGObject::setBRender(bool bRender)
{
	_skyboxFObj->setBRender(bRender);
}

void SkyboxGObject::preMade()
{
	SHADER::ShaderSkybox* skyboxShader = GShaderManager->m_addShader<SHADER::ShaderSkybox>(ENUM_SHADER_TYPE::SHADER_TYPE_SKYBOX, "data/Shader/SkyboxMapping.vertexshader", "data/Shader/SkyboxMapping.fragmentshader");
	RENDER_TARGET::SKYBOX::SkyboxFObj* tempSkyboxFObj = new RENDER_TARGET::SKYBOX::SkyboxFObj("data/Texture/sky_bot.dds", "data/Texture/sky_top.dds", "data/Texture/sky_back.dds", "data/Texture/sky_front.dds", "data/Texture/sky_right.dds", "data/Texture/sky_left.dds");
	tempSkyboxFObj->setBRender(false);
	_preMadeSpaceSkybox.push_back(new SkyboxGObject(nullptr, skyboxShader, tempSkyboxFObj));
}

void SkyboxGObject::setCollisionTest(bool bCollision)
{
}

void SkyboxGObject::doJobWithBeDeleted()
{
}

