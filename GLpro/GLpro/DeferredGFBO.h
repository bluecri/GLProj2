#pragma once

#include "stdafx.h"
#include "PostGraphicProcess.h"

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
		void initDeferredGFBO();
		
		void deferredPreDraw(float deltaTime);
		void deferredDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList);
		void deferredAfterDraw(float deltaTime);
		void deferredDrawToScreen(float deltaTime);	// for DEBUG

		void postProcessDraw(float deltaTime);	// for DEBUG

		float calcExposureWithMipmap();


		void modeForGeoDraw();
		void modeForAfterDraw();
		void modeForSkybox();
		void modeForParticle();

		void bindGFBO_RESULT();
		void unbindGFBO_RESULT();

	protected:
		void createBuffer();

		void bindShadowFBO();
		void unbindShadowFBO();
		
		void bindGFBO_GEO();
		void unbindGFBO_GEO();

		void bindGFBO_LIGHT();
		void unbndGFBO_LIGHT();

		void renderGFBOToScreen();

		void bndGFBO_FINAL();
		void unbndGFBO_FINAL();

		void shadowDraw(float deltaTime);
		void geoDraw(float deltaTime, std::list<std::shared_ptr<std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>>>& drawObjList);
		void lightDraw(float deltaTime);
		void finalDraw(float deltaTime);


		SHADER::ShaderShadow* getShaderDhadow();
		SHADER::ShaderGBufferGeo* getShaderGFBOGeo();
		SHADER::ShaderGBufferLight* getShaderGFBOLight();
		SHADER::ShaderGBufferFinal* getShaderGFBOFinal();

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
		GLuint	_GFBODepthStencilTexture;		// not use in render pipeline

		GLuint	_GFBOBloomTexture;
		GLuint	_GFBOResultTexture;

		int _shadowTextureFBOX, _shadowTextureFBOY;
		int _GBOX, _GBOY;

		PostGraphicProcess _postGraphicProcess;
	};
}

extern RESOURCE::DeferredGFBO* GDeferredGFBO;