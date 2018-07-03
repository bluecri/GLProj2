#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include "./ShaderHeader.h"
#include "./ShaderObj.h"

#include <vector>
#include <map>

enum ENUM_SHADER_TYPE
{
	SHADER_TYPE_MAIN = 0,
	SHADER_TYPE_SHADOW = 1,
	SHADER_TYPE_COLISION = 2,
	SHADER_TYPE_SKYBOX = 3,
	SHADER_TYPE_PARTICLE = 4,
	SHADER_TYPE_TEXT = 5,
	SHADER_TYPE_TEXTURESIMPLE = 6, 
	SHADER_TYPE_GBUFFER_GEO,
	SHADER_TYPE_GBUFFER_LIGHT,
	SHADER_TYPE_GBUFFER_FINAL,
	SHADER_TYPE_NUM
};

using ShaderElem = std::map<std::string, SHADER::ShaderObj*>;
using ShaderElemContainer = std::vector<ShaderElem>;

namespace SHADER
{
	class ShaderManager {
	public:

		ShaderManager();

		//todo : check T -> shaderObj in static time
		template<class T>
		T* m_addShader(ENUM_SHADER_TYPE type, const char * vertexShader, const char * fragmentShader)
		{
			std::string keyStr(vertexShader);
			keyStr.append(fragmentShader);
			auto elem = m_shaderStorage[type].find(keyStr);

			if (m_shaderStorage[type].end() != elem)
			{
				return static_cast<T*>(elem->second);	// already exist
			}

			ShaderObj* tempShader = new T(vertexShader, fragmentShader);
			if (tempShader->getShaderID() != -1) {
				m_shaderStorage[type].insert(std::make_pair(keyStr, tempShader));
				return static_cast<T*>(tempShader);
			}
			
			return nullptr;
		}

		void removeShaderPtr(ENUM_SHADER_TYPE type, const char * vertexShader, const char * fragmentShader);

		void removeShaderPtr(ENUM_SHADER_TYPE type, ShaderObj* delShaderObj);

		virtual ~ShaderManager();
	private:
		ShaderElemContainer m_shaderStorage;

	};
}
extern SHADER::ShaderManager* GShaderManager;

#endif
