#ifndef __SHADER_OBJ_H__
#define __SHADER_OBJ_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <vector>

#include "../../3rdParty/ShaderLoader/shader.h"

namespace SHADER
{
	class ShaderObj
	{
		public:
			GLuint m_MVPMatrixID;
			GLuint m_cameraViewMatrixID;
			GLuint m_modelMatrixID;
			GLuint m_depthBiasID;
			GLuint m_shadowMapID;
			GLuint m_lightInvDirID;
			GLuint m_textureID;

			GLuint depthMatrixID;

			GLuint m_text2DUniformID;

			//particle
			GLuint m_cameraRight_worldspace_ID;
			GLuint m_cameraUp_worldspace_ID;
			GLuint m_viewProjMatrixID;

			ShaderObj(const char * vertexShader, const char * fragmentShader) {
				m_shaderID = LoadShaders(vertexShader, fragmentShader);

				m_MVPMatrixID = -1;
				m_cameraViewMatrixID = -1;
				m_modelMatrixID = -1;
				m_depthBiasID = -1;
				m_shadowMapID = -1;
				m_lightInvDirID = -1;
				m_textureID = -1;

				depthMatrixID = -1;

				m_text2DUniformID = -1;

				m_cameraRight_worldspace_ID = -1;
				m_cameraUp_worldspace_ID = -1;
				m_viewProjMatrixID = -1;
			}
			~ShaderObj()
			{
				glDeleteProgram(m_shaderID);
			}

			void bind() {
				glUseProgram(m_shaderID);
			};
			void unbind() {
				glUseProgram(0);
			};

			virtual ~ShaderObj() {
				glDeleteProgram(m_shaderID);
			};

			GLuint getShaderID() {
				return m_shaderID;
			}

			void loadInt(GLuint location, int value)
			{
				glUniform1i(location, value);
			}

			void loadFloat(GLuint location, float value)
			{
				glUniform1f(location, value);
			}

			void loadVector2(GLuint location, const glm::vec2& vector)
			{
				glUniform2f(location, vector.x, vector.y);
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
