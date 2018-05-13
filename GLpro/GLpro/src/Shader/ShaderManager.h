#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include "./ShaderHeader.h"
namespace SHADER 
{
	class ShaderManager {
	public:
		std::vector<ShaderObj*> m_shaderStorage;

		enum ENUM_SHADER_TYPE
		{
			MAIN = 0, SHADOW = 1, COLISION = 2, /* SKYBOX = 3, PARTICLE = 4, TEXT = 5,*/ SHADER_TYPE_NUM
		};

		ShaderManager()
		{
			m_shaderStorage.reserve(SHADER_TYPE_NUM);
			m_shaderStorage.push_back(new ShaderMain("shader/ShadowMapping.vertexshader", "shader/ShadowMapping.fragmentshader"));
			m_shaderStorage.push_back(new ShaderShadow("shader/DepthRTT.vertexshader", "shader/DepthRTT.fragmentshader"));
			m_shaderStorage.push_back(new ShaderMain("shader/Simple.vertexshader", "shader/Simple.fragmentshader"));
		}

		template<typename T>
		ShaderObj* m_addShader<T>(const char * vertexShader, const char * fragmentShader)
		{
			T* tempShader = new T(vertexShader, fragmentShader);
			if (tempShader->m_shaderID != -1) {
				m_shaderStorage.push_back(tempShader);
				return tempShader;
			}
			else {
				return nullptr;
			}
		}

		ShaderObj* getShaderPtrWithint(int idx) const 
		{
			return m_shaderStorage[idx];
		}

		virtual ~ShaderManager() 
		{
			for (auto elem : m_shaderStorage)
			{
				delete elem;
			}
		}
	};
}


#endif
