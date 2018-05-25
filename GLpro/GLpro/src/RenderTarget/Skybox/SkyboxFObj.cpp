#include "../../../stdafx.h"
#include "SkyboxFObj.h"

#include "./SkyboxModel.h"
#include "./SkyboxTexture.h"

RENDER_TARGET::SKYBOX::SkyboxFObj::SkyboxFObj(const char * up_path, const char * down_path, const char * left_path, const char * right_path, const char * front_path, const char * back_path, float dist)
	: FObj()
{
	_skbTexture = new SkyboxTexture(up_path, down_path, left_path, right_path, front_path, back_path);
	_skbModel = new SkyboxModel(dist);
}

RENDER_TARGET::SKYBOX::SkyboxFObj::~SkyboxFObj() {	}
