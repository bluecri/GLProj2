#pragma once
#include "stdafx.h"
#include "IMissileInfo.h"
#include "NormalMissileState.h"

class NormalMissileInfo :public IMissileInfo
{
public:
	NormalMissileInfo()
	{
		_commonMissileInfo._modelFileName = std::string("data/Model/missile.obj");
		_commonMissileInfo._textureFileName = std::string("data/Texture/block.dds");
		_commonMissileInfo._textureTypeName = std::string("dds");
		_commonMissileInfo._shaderType = ENUM_SHADER_TYPE::SHADER_TYPE_MAIN;
		_commonMissileInfo._shaderVertexFileName = std::string("data/Shader/ShadowMapping.vertexshader");
		_commonMissileInfo._shaderFragmentFileName = std::string("data/Shader/ShadowMapping.fragmentshader");
				
		_commonMissileInfo._startSound = std::string("laser");
		_commonMissileInfo._hitSound = std::string("hit");

		_commonMissileInfo._genPosAdd = glm::vec3(0.0f, 0.0f, 0.2f);
		_commonMissileInfo._lifeTime = 4.0f;
	}

	NormalMissileState normalMissileState;
};