#include "../../stdafx.h"
#include "ShaderManager.h"


SHADER::ShaderManager::ShaderManager()
{
	for (int i = 0; i < ENUM_SHADER_TYPE::SHADER_TYPE_NUM; i++)
	{
		m_shaderStorage.push_back(std::map<std::string, SHADER::ShaderObj*>());
	}

	// todo : template class·Î ¹Ù²ãº¼°Í
	m_addShader<SHADER::ShaderMain>(SHADER_TYPE_MAIN, "data/Shader/ShadowMapping.vertexshader", "data/Shader/ShadowMapping.fragmentshader");
	m_addShader<SHADER::ShaderShadow>(SHADER_TYPE_SHADOW, "data/Shader/DepthRTT.vertexshader", "data/Shader/DepthRTT.fragmentshader");
	//m_shaderStorage[SHADER_TYPE_COLISION].push_back(new ShaderMain("shader/Simple.vertexshader", "shader/Simple.fragmentshader"));

}

void SHADER::ShaderManager::removeShaderPtr(ENUM_SHADER_TYPE type, const char * vertexShader, const char * fragmentShader)
{
	std::string keyStr(vertexShader);
	keyStr.append(fragmentShader);
	auto elem = m_shaderStorage[type].find(keyStr);

	if (m_shaderStorage[type].end() != elem)
	{
		m_shaderStorage[type].erase(elem);
		return;
	}
	return;
}

void SHADER::ShaderManager::removeShaderPtr(ENUM_SHADER_TYPE type, ShaderObj * delShaderObj)
{
	auto elem = m_shaderStorage[type].find(delShaderObj->getKeyStr());

	if (m_shaderStorage[type].end() != elem)
	{
		m_shaderStorage[type].erase(elem);
		return;
	}
}

SHADER::ShaderManager::~ShaderManager()
{
	for (auto elem : m_shaderStorage)
	{
		for (auto inElem : elem)
		{
			delete inElem.second;
		}
	}
}

SHADER::ShaderManager* GShaderManager = nullptr;