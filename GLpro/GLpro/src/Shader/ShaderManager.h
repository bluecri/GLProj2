#ifndef __SHADER_MANAGER_H__
#define __SHADER_MANAGER_H__

#include "./ShaderHeader.h"
#include <memory>
namespace SHADER 
{
	class ShaderManager {
	public:
		std::vector<ShaderObj*> m_shaderStorage;

		enum ENUM_SHADER_IDX
		{
			//MAIN = 0, SHADOW = 1, COLISION = 2
		};
		ShaderManager() {
			//m_addMainShader("shader/ShadowMapping.vertexshader", "shader/ShadowMapping.fragmentshader");
			//m_addShadowShader("shader/DepthRTT.vertexshader", "shader/DepthRTT.fragmentshader");
			//m_addMainShader("shader/Simple.vertexshader", "shader/Simple.fragmentshader");
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

		ShaderObj* getShaderPtrWithint(int idx) const {
			return m_shaderStorage[idx];
		}
		virtual ~ShaderManager() {

		}
	};
}


#endif
