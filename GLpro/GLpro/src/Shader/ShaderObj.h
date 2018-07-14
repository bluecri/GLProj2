#ifndef __SHADER_OBJ_H__
#define __SHADER_OBJ_H__

#include "../../stdafx.h"

namespace SHADER
{
	class ShaderObj
	{
	public:
		ShaderObj(const char * vertexShader, const char * fragmentShader);
		virtual ~ShaderObj();

		void bind();
		void unbind();
		GLuint getShaderID();

		void loadInt(GLuint location, int value);
		void loadBool(GLuint location, bool b);
		void loadVector2i(GLuint location, const int& int1, const int& int2);
		void loadFloat(GLuint location, float value);
		void loadVector2(GLuint location, const glm::vec2& vector);
		void loadVector3(GLuint location, const glm::vec3& vector);
		void loadVector3f(GLuint location, const glm::vec3 & vector);
		void loadVector3(GLuint location, const float& x, const float& y, const float& z);
		void loadMatrix4(GLuint location, const glm::mat4& matrix, int count = 1);

		void uniformBlockBind(GLuint location, int uniformIndex);

		std::string& getKeyStr();

	protected:
		GLuint m_shaderID;
		std::string _keyStr;
	};

}
#endif
