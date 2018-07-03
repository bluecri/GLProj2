#pragma once

#include "stdafx.h"

namespace SHADER
{
	class ShaderShadow;
	class ShaderGBufferGeo;
	class ShaderGBufferLight;
	class ShaderGBufferFinal;
}

namespace RENDER
{
	class RNormal;
}

namespace RENDER_TARGET {
	namespace NORMAL
	{
		class NormalFObj;
	}
}

class RigidbodyComponent;

namespace RESOURCE
{
	class DeferredGFBO
	{
	public:
		DeferredGFBO(int screenSizeX, int screenSizeY);
		virtual ~DeferredGFBO();
		void init();

		void deferredDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList);

	protected:
		void createBuffer();

		void bindShadowFBO();
		void unbindShadowFBO();
		SHADER::ShaderShadow* getShaderDhadow();

		void bindGFBO_GEO();
		void unbindGFBO_GEO();
		SHADER::ShaderGBufferGeo* getShaderGFBOGeo();

		void bindGFBO_LIGHT();
		void unbndGFBO_LIGHT();
		SHADER::ShaderGBufferLight* getShaderGFBOLight();

		void renderGFBOToScreen();

		void bndGFBO_FINAL();
		void unbndGFBO_FINAL();
		SHADER::ShaderGBufferFinal* getShaderGFBOFinal();

		void setRenderScreenDirect(bool bDirect);
		bool getRenderScreenDirect();

		void shadowDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList);
		void geoDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList);
		void lightDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList);
		void finalDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList);

	private:
		SHADER::ShaderShadow*		_shadowShader;
		SHADER::ShaderGBufferGeo*	_geoShader;
		SHADER::ShaderGBufferLight*	_lightShader;
		SHADER::ShaderGBufferFinal*	_finalShader;

		GLuint	_lightsShadowFBO;
		GLuint	_lightsShadowDepthTexture;

		GLuint	_GFBO;
		GLuint	_GFBOWorldPosTexture;
		GLuint	_GFBOColorTexture;
		GLuint	_GFBONormalTexture;
		GLuint	_GFBODepthTexture;		// not use in render pipeline

		GLuint	_GFBOResultTexture;

		int _shadowTextureFBOX, _shadowTextureFBOY;
		int _GBOX, _GBOY;

		bool	_bRenderOnScreenDirect;
	};
}

extern RESOURCE::DeferredGFBO* GDeferredGFBO;