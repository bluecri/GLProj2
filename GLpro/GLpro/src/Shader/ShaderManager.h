#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include "./ShaderHeader.h"
#include "./ShaderObj.h"

#include <vector>
#include <map>

enum ENUM_SHADER_TYPE
{
	SHADER_TYPE_MAIN = 0, SHADER_TYPE_SHADOW = 1, SHADER_TYPE_COLISION = 2, SHADER_TYPE_SKYBOX = 3, SHADER_TYPE_PARTICLE = 4, SHADER_TYPE_TEXT = 5, SHADER_TYPE_NUM
};

using ShaderElem = std::map<std::string, SHADER::ShaderObj*>;
using ShaderElemContainer = std::vector<ShaderElem>;

namespace SHADER
{
	class ShaderManager {
	public:
		ShaderElemContainer m_shaderStorage;

		ShaderManager()
		{
			for (int i = 0; i < ENUM_SHADER_TYPE::SHADER_TYPE_NUM; i++)
			{
				m_shaderStorage.push_back(std::map<std::string, SHADER::ShaderObj*>());
			}
			m_shaderStorage[SHADER_TYPE_MAIN].insert(std::make_pair("shader/ShadowMapping.vertexshader,shader/ShadowMapping.fragmentshader", new ShaderMain("shader/ShadowMapping.vertexshader", "shader/ShadowMapping.fragmentshader")));
			m_shaderStorage[SHADER_TYPE_SHADOW].insert(std::make_pair("shader/DepthRTT.vertexshader,shader/DepthRTT.fragmentshade", new ShaderShadow("shader/DepthRTT.vertexshader", "shader/DepthRTT.fragmentshader")));
			//m_shaderStorage[SHADER_TYPE_COLISION].push_back(new ShaderMain("shader/Simple.vertexshader", "shader/Simple.fragmentshader"));
		}

		
		template<class T>
		ShaderObj* m_addShader(ENUM_SHADER_TYPE type, const char * vertexShader, const char * fragmentShader)
		{
			std::string keyStr(vertexShader);
			keyStr.append(fragmentShader);
			auto elem = m_shaderStorage[type].find(keyStr);

			if (m_shaderStorage[type].end() != elem)
			{
				return elem->second;	// already exist
			}

			ShaderObj* tempShader = new T(vertexShader, fragmentShader);
			if (tempShader->m_shaderID != -1) {
				m_shaderStorage[type].push_back(tempShader);
				return tempShader;
			}
			
			return nullptr;
		}
		
		ShaderObj* getShaderPtr(ENUM_SHADER_TYPE type, const char * vertexShader, const char * fragmentShader) 
		{
			std::string keyStr(vertexShader);
			keyStr.append(fragmentShader);
			auto elem = m_shaderStorage[type].find(keyStr);

			if (m_shaderStorage[type].end() != elem)
			{
				return elem->second;
			}

			return nullptr;
		}

		void removeShaderPtr(ENUM_SHADER_TYPE type, const char * vertexShader, const char * fragmentShader) 
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

		void removeShaderPtr(ENUM_SHADER_TYPE type, ShaderObj* delShaderObj)
		{
			auto elem = m_shaderStorage[type].find(delShaderObj->getKeyStr());

			if (m_shaderStorage[type].end() != elem)
			{
				m_shaderStorage[type].erase(elem);
				return;
			}

		}

		virtual ~ShaderManager() 
		{
			for (auto elem : m_shaderStorage)
			{
				for (auto inElem : elem)
				{
					delete inElem.second;
				}
			}
		}
	};
}


#endif
