#pragma once

#include "src/Entity.h"
#include "src/Shader/ShaderManager.h"
#include "src/Shader/ShaderSkybox.h"
#include "RenderManager.h"
#include "src/Render/RSkybox.h"
#include "src/RenderTarget/Skybox/SkyboxFObj.h"

class SpaceSkybox : public Entity
{
public:
	SpaceSkybox()
	{
		SHADER::ShaderSkybox* skyboxShader = GShaderManager->m_addShader<SHADER::ShaderSkybox>(ENUM_SHADER_TYPE::SHADER_TYPE_SKYBOX, "shader/SkyboxMapping.vertexshader", "shader/SkyboxMapping.fragmentshader");
		rSkybox = GRendermanager->getRRender<RENDER::RSkybox, SHADER::ShaderSkybox>(skyboxShader);
		skyboxFObj = new RENDER_TARGET::SKYBOX::SkyboxFObj("texture/sky_bot.dds", "texture/sky_top.dds", "texture/sky_back.dds", "texture/sky_front.dds", "texture/sky_right.dds", "texture/sky_left.dds", 40.0f);
		registeredElemInRenderer = rSkybox->addToDrawList(skyboxFObj, _rigidbodyComponent);
	}
	~SpaceSkybox()
	{
		rSkybox->destructor(registeredElemInRenderer);
		delete skyboxFObj;
	}

	static void preMade()
	{
		preMadeSpaceSkybox.push_back(new SpaceSkybox());
	}

public:
	RENDER::RSkybox* rSkybox;
	RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxFObj;
	static std::vector<SpaceSkybox*> preMadeSpaceSkybox;
private:
	std::shared_ptr<RENDER::RSkybox::DrawElement> registeredElemInRenderer;
};