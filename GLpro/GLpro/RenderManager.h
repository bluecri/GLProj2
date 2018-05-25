#pragma once

#include <map>
#include <type_traits>
#include <string>

#include "./src/window.h"

#include "RBox.h"
#include "src/Render/RNormal.h"
#include "src/Render/RSkybox.h"
#include "RText.h"
#include "RParticle.h"


namespace SHADER
{
	class ShaderObj;
}

enum RRENDER_TYPE
{
	RRENDER_TYPE_RBOX,
	RRENDER_TYPE_RNormal,
	RRENDER_TYPE_RParticle,
	RRENDER_TYPE_RSkybox,
	RRENDER_TYPE_RText,
	RRENDER_TYPE_NUM
};

// shader의 구분은 shader file name으로 / renderer의 구분은 shader*로
// shader의 관리는 shader manager / renderer의 관리는 renderer manager

namespace RENDER
{

	// TODO : RRenerer is base of T (type check)
	template<class T, class K>	// T = R*****(renderer class), K== shaderObj(child)
	class RRenderContainerClass
	{
	public:
		// shader로 renderer 생성 or 기존의 renderer 반환
		T* addOrGetRenderer(K* inShaderPtr);
		// renderer 제거.
		void removeRenderer(T* delRenderer);
		void render(float deltaTime);
	public:
		std::map<K*, T*> _rRenderContainer;
	};

	// Use TextFObj & ShaderText(widthLT, heightLT)
	class RenderManager
	{
	private:
		// RRender Containers
		RRenderContainerClass<RBox, SHADER::ShaderText> _boxContainer;
		RRenderContainerClass<RNormal, SHADER::ShaderMain> _normalContainer;
		RRenderContainerClass<RParticle, SHADER::ShaderParticle> _particleContainer;
		RRenderContainerClass<RSkybox, SHADER::ShaderSkybox> _skyboxContainer;
		RRenderContainerClass<RText, SHADER::ShaderText> _textContainer;

	public:
		RenderManager()
		{
		}

		// todo : acc 처리
		void renderAll(float deltaTime, float acc);
		void renderBufferInit()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, GWindow->_windowWidth, GWindow->_windowHeight); // Render on the whole framebuffer, complete from the lower left corner to the upper right
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glCullFace(GL_BACK); // Cull back-facing triangles -> draw only front-facing triangles
		}

		void swapRenderBuffer();

		// T = RenderClass , K shaderObj children
		template<class T, class K>
		typename std::enable_if<std::is_same<T, RENDER::RNormal>::value, T*>::type
			getRRender(K* shaderObj) {
			return _normalContainer.addOrGetRenderer(shaderObj);
		}

		template<class T, class K>
		typename std::enable_if<std::is_same<T, RENDER::RBox>::value, T*>::type
			getRRender(K* shaderObj) {
			return _boxContainer.addOrGetRenderer(shaderObj);
		}

		template<class T, class K>
		typename std::enable_if<std::is_same<T, RENDER::RSkybox>::value, T*>::type
			getRRender(K* shaderObj) {
			return _skyboxContainer.addOrGetRenderer(shaderObj);
		}

		template<class T, class K>
		typename std::enable_if<std::is_same<T, RENDER::RText>::value, T*>::type
			getRRender(K* shaderObj) {
			return _textContainer.addOrGetRenderer(shaderObj);
		}

		template<class T, class K>
		typename std::enable_if<std::is_same<T, RENDER::RParticle>::value, T*>::type
			getRRender(K* shaderObj) {
			return _particleContainer.addOrGetRenderer(shaderObj);
		}

		// K = RenderClass , T shaderObj children
		/*	nextTodo : change shader(inner obj : shader(renderer) -> shader(renderer))
		template<class K, class T>
		typename std::enable_if<std::is_same<K, RENDER::RNormal>::value, K*>::type
			changeRRender(K* oldRenderer, T* targetShaderObj) {
			static_assert(std::is_same<K::TYPE_SHADER, T>::value, "getRRender RNormal type static assert");
			oldRenderer->
		}
		*/
	};

	// shader로 renderer 생성 or 기존의 renderer 반환
	template<class T, class K>
	T * RRenderContainerClass<T, K>::addOrGetRenderer(K * inShaderPtr)
	{
		auto it = _rRenderContainer.find(inShaderPtr);
		if (it != _rRenderContainer.end())
		{
			return (*it).second;
		}
		T* newRenderer = new T(inShaderPtr);
		_rRenderContainer.insert(std::make_pair(inShaderPtr, newRenderer));

		return newRenderer;
	}

	// renderer 제거.
	template<class T, class K>
	void RRenderContainerClass<T, K>::removeRenderer(T * delRenderer)
	{
		auto it = _rRenderContainer.find(delRenderer->getShader());
		if (it != _rRenderContainer.end())
		{
			delete it.second;
			return;
		}
	}

	template<class T, class K>
	void RRenderContainerClass<T, K>::render(float deltaTime)
	{
		for (auto elem : _rRenderContainer)
		{
			elem.second->draw(deltaTime);
		}
	}
	
}

extern RENDER::RenderManager* GRendermanager;