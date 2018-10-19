#pragma once

#include "stdafx.h"

#define PBO_SIZE 2

namespace SHADER
{
	class ShaderShadow;
	class ShaderGBufferGeo;
	class ShaderGBufferLight;
	class ShaderGBufferFinal;

	class ShaderTextureSimple;
	class ShaderHDR; 
	class ShaderBloom;
	class ShaderFXAA;
	class ShaderMipmap;
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
	class ModelOnlyVertex;
}

enum ENUM_POST_PIPELINE {
	POST_PIPELINE_PREV,
	POST_PIPELINE_HDR,
	POST_PIPELINE_FXAA,
	POST_PIPELINE_POST,
	POST_PIPELINE_SCREEN
};

namespace RESOURCE
{
	class PostGraphicProcess
	{
	public:
		PostGraphicProcess(int GBOX, int GBOY);
		virtual ~PostGraphicProcess();
		void initPostGraphicProcess();

		void beforePostGraphicProcess(GLuint GFBORetTextureID, GLuint bloomTextureID);
		void doPostGraphicProcess();

		float getExposure();
		void refreshRenderPipeline();
	protected:
		void createBuffer();

		void bindTargetTextureHDR();
		void bindTargetTextureFXAA();
		void bindTargetTexturePostEffect();
		void unbindTargetTexture();

		void bindReadTextureGFBO();
		void bindReadTextureHDR();
		void bindReadTextureFXAA();
		void bindReadTexturePostEffect();

		void bindSimpleShader();
		void bindHDRShader();
		void unbindHDRShader();
		void bindFXAAShader();
		void unbindFXAAShader();

		void hdrDraw();
		void fxaaDraw();
		// void postEffectDraw(float deltaTime);
		void screenDraw();

		// void setExposure(float exp);

		void calcExposure(float newExposure);
		void setExposure(float exp);
		void setExposureAdjustSpeed(float adjustSpeed);
		float getExposureAdjustSpeed();


		void GenerateMipmapExposure();
		float calcExposureWithMipmap();
		float calcExposureWithMipmapReadPixel();
		
		bool postGraphicProcessLoop();
		void doBlooming();

		void bloomingFirst();
		void bloomingVert();
		void bloomingHor();

	private:
		SHADER::ShaderTextureSimple* _shaderTextureSimple;
		SHADER::ShaderHDR*		_hdrShader;
		SHADER::ShaderBloom*	_bloomShader;
		SHADER::ShaderFXAA*		_fxaaShader;
		SHADER::ShaderMipmap*	_mipmapShader;
		// SHADER::ShaderPostEffect*	_postEffectShader;

		GLuint	_GFBORetTextureID;
		GLuint	_bloomBeforeTexture;

		GLuint	_FBO;
		GLuint	_FBOHDRTexture;
		
		GLuint	_FBOBloomVertTexture;
		GLuint	_FBOBloomHorTexture;
		GLuint	_FBOFXAATexture;
		GLuint	_FBOPostEffectTexture;

		GLuint	_FBOForMipmap;
		GLuint	_FBOLastMipmapTexture;
		int		_mipmapLevel;

		GLuint	_pixelBufferID[PBO_SIZE];
		GLsync	_fenceSync[2];
		int		_swapPBOIndex;
		int		_swapPBOSize;
		int		_exposureUpdateInterval;
		int		_curExposureUpdateInterval;
		bool	_bFirstTry;

		int _GBOX, _GBOY;

		float	_exposure;
		float	_prevExposure;
		float	_exposureAdjustSpeed;

		std::vector<int>	_renderPipelineVec;
		int					_renderPipePrevIdx;

		ModelOnlyVertex*	_modelOnlyVertex;


		GLuint _emptyTestTexture;
	};
}