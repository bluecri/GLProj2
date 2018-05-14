#ifndef __SHADER_OBJ_H__
#define __SHADER_OBJ_H__

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
		void loadFloat(GLuint location, float value);
		void loadVector2(GLuint location, const glm::vec2& vector);
		void loadVector3(GLuint location, const glm::vec3& vector);
		void loadMatrix4(GLuint location, const glm::mat4& matrix);

	protected:
		GLuint m_shaderID;
	};

}
#endif
