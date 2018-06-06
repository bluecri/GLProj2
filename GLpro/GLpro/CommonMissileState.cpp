#include "stdafx.h"
#include "CommonMissileState.h"
#include "ICommonMissileInfo.h"

#include "./src/Resource/Model.h"
#include "./src/Resource/ModelManager.h"
#include "./src/Resource/Texture.h"
#include "./src/Resource/TextureManager.h"
#include "./src/Shader/ShaderManager.h"
#include "./src/Shader/ShaderMain.h"

CommonMissileState::CommonMissileState()
{
	_missileModel = nullptr;
	_missileTexture = nullptr;
	_missileShaderMain = nullptr;

	_shotDelay = 0.0f;

}

void CommonMissileState::initCommonState(ICommonMissileInfo * commonMissileInfo)
{
	_missileModel = GModelManager->getModelWithFileName(commonMissileInfo->_modelFileName);
	_missileTexture = GTextureManager->getTextureWithFileName(commonMissileInfo->_textureFileName, commonMissileInfo->_textureTypeName);
	_missileShaderMain = GShaderManager->m_addShader<SHADER::ShaderMain>(ENUM_SHADER_TYPE::SHADER_TYPE_MAIN, commonMissileInfo->_shaderVertexFileName.c_str(), commonMissileInfo->_shaderFragmentFileName.c_str());

	_shotDelay = commonMissileInfo->_shotDelay;
}
