#pragma once

#include "stdafx.h"
#include "src/Entity.h"
#include "src/Render/RSkybox.h"

namespace SHADER { class ShaderSkybox; }
namespace RENDER_TARGET { namespace SKYBOX { class SkyboxFObj; } }


class SkyboxGObject : public Entity
{
public:
	SkyboxGObject(SHADER::ShaderSkybox* shader, RENDER_TARGET::SKYBOX::SkyboxFObj* skyboxFObj);
	~SkyboxGObject();

	virtual void setBRender(bool bRender);

	static void preMade();

public:
	RENDER::RSkybox* rSkybox;
	RENDER_TARGET::SKYBOX::SkyboxFObj* _skyboxFObj;
	static std::vector<SkyboxGObject*> _preMadeSpaceSkybox;
private:
	std::shared_ptr<RENDER::RSkybox::DrawElement> registeredElemInRenderer;
};

