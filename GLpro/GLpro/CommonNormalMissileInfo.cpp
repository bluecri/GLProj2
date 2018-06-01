#include "stdafx.h"
#include "CommonNormalMissileInfo.h"
#include "./src/Shader/ShaderManager.h"

void CommonNormalMissileInfo::init()
{
	_modelFileName = std::string("data/Model/missile.obj");
	_textureFileName = std::string("data/Texture/uvmap.dds");
	_textureTypeName = std::string("dds");
	_shaderType = ENUM_SHADER_TYPE::SHADER_TYPE_MAIN;
	_shaderVertexFileName = std::string("data/Shader/ShadowMapping.vertexshader");
	_shaderFragmentFileName = std::string("data/Shader/ShadowMapping.fragmentshader");

	_shotDelay = 0.1f;

	_collisionBoxMat = glm::mat4();
	_missileCollisionBoxAxis = glm::vec3(0.1f, 0.1f, 1.0f);
}

void CommonNormalMissileInfo::init(std::string & commonMissileInfoFile)
{
	// read file & get info
}
