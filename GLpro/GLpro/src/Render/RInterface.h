#ifndef __RRENDER_INTERFACE_H__
#define __RRENDER_INTERFACE_H__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "../Shader/ShaderObj.h"


class RInterface 
{
	public:
		virtual ~RInterface() {};
		virtual void skyboxUpdate() = 0;
		virtual void skyboxDraw() = 0;

		virtual void chageShader(SHADER::ShaderObj* other)
		{
			_shaderObj = other;
		}
		virtual SHADER::ShaderObj* getShader() const 
		{
			return _shaderObj;
		}
	protected:
		SHADER::ShaderObj* _shaderObj;

	private:
	
};

#endif