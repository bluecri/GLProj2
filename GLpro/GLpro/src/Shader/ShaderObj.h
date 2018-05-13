#ifndef __SHADER_OBJ_H__
#define __SHADER_OBJ_H__

#include "../../stdafx.h"

namespace SHADER
{
	class ShaderObj
	{
	public:
		ShaderObj(const char * vertexShader, const char * fragmentShader) {
			m_shaderID = LoadShaders(vertexShader, fragmentShader);
		}
		virtual ~ShaderObj() { glDeleteProgram(m_shaderID); }

		void bind()		{ glUseProgram(m_shaderID); }
		void unbind()	{ glUseProgram(0); }
		GLuint getShaderID()	{ return m_shaderID; }

		void loadInt(GLuint location, int value)	 { glUniform1i(location, value); }
		void loadFloat(GLuint location, float value) { glUniform1f(location, value); }

		void loadVector2(GLuint location, const glm::vec2& vector)
		{
			glUniform2f(location, vector.x, vector.y);
		}

		void loadVector3(GLuint location, const glm::vec3& vector)
		{
			glUniform3f(location, vector.x, vector.y, vector.z);
		}

		void loadMatrix4(GLuint location, const glm::mat4& matrix)
		{
			glUniformMatrix4fv(location, 1, GL_FALSE, (GLfloat*)&matrix);
		}

	protected:
		GLuint m_shaderID;
	};

}
#endif
