#include "stdafx.h"
#include "CommonMissileState.h"
#include "CommonNormalMissileInfo.h"


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

	_missileDelay = 0.0f;

}

void CommonMissileState::init(CommonNormalMissileInfo * commonNormalMissileInfo)
{
	_missileModel = GModelManager->getModelWithFileName(commonNormalMissileInfo->_modelFileName);
	_missileTexture = GTextureManager->getTextureWithFileName(commonNormalMissileInfo->_textureFileName, commonNormalMissileInfo->_textureTypeName);
	_missileShaderMain = GShaderManager->m_addShader<SHADER::ShaderMain>(commonNormalMissileInfo->_shaderType, commonNormalMissileInfo->_shaderVertexFileName.c_str(), commonNormalMissileInfo->_shaderFragmentFileName.c_str());

	_missileDelay = commonNormalMissileInfo->_shotDelay;
}
