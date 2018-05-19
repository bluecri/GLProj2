#pragma once

#include <map>
#include <type_traits>
#include <string>

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
	template<class T>	// T = R*****(renderer class)
	class RRenderContainerClass
	{
		using TYPE_SHADER = T::TYPE_SHADER;
	private:
		std::map<T::TYPE_SHADER*, T*> _rRenderContainer;		// 아래에 있으면 안잡힘..

	public:
		// shader로 renderer 생성 or 기존의 renderer 반환
		T* addOrGetRenderer(TYPE_SHADER* inShaderPtr)
		{
			auto it = _rRenderContainer.find(inShaderPtr);
			if (it != _rRenderContainer.end())
			{
				return it.second;
			}
			T* newRenderer = new T(inShaderPtr);
			_rRenderContainer.insert(std::make_pair(inShaderPtr, newRenderer));

			return newRenderer;

		}
		// renderer 제거.
		void removeRenderer(T* delRenderer)
		{
			auto it = _rRenderContainer.find(delRenderer->getShader());
			if (it != _rRenderContainer.end())
			{
				delete it.second;
				return;
			}
		}
	};

	// Use TextFObj & ShaderText(widthLT, heightLT)
	class RenderManager
	{
	private:
		// RRender Containers
		RRenderContainerClass<RBox> _boxContainer;
		RRenderContainerClass<RNormal> _normalContainer;
		RRenderContainerClass<RParticle> _particleContainer;
		RRenderContainerClass<RSkybox> _skyboxContainer;
		RRenderContainerClass<RText> _textContainer;

	public:
		RenderManager()
		{

		}

		// K = RenderClass , T shaderObj children
		template<class K, class T>
		typename std::enable_if<std::is_same<K, RENDER::RNormal>::value, K*>::type
			getRRender(T* shaderObj) {
			static_assert(std::is_same<K::TYPE_SHADER, T>::value, "getRRender RNormal type static assert");
			_normalContainer.addOrGetRenderer(shaderObj);
		}

		template<class K, class T>
		typename std::enable_if<std::is_same<K, RENDER::RBox>::value, K*>::type
			getRRender(T* shaderObj) {
			static_assert(std::is_same<K::TYPE_SHADER, T>::value, "getRRender RBox type static assert");
			_boxContainer.addOrGetRenderer(shaderObj);
		}

		template<class K, class T>
		typename std::enable_if<std::is_same<K, RENDER::RSkybox>::value, K*>::type
			getRRender(T* shaderObj) {
			static_assert(std::is_same<K::TYPE_SHADER, T>::value, "getRRender RSkybox type static assert");
			_skyboxContainer.addOrGetRenderer(shaderObj);
		}

		template<class K, class T>
		typename std::enable_if<std::is_same<K, RENDER::RText>::value, K*>::type
			getRRender(T* shaderObj) {
			static_assert(std::is_same<K::TYPE_SHADER, T>::value, "getRRender RText type static assert");
			_textContainer.addOrGetRenderer(shaderObj);
		}

		template<class K, class T>
		typename std::enable_if<std::is_same<K, RENDER::RParticle>::value, K*>::type
			getRRender(T* shaderObj) {
			static_assert(std::is_same<K::TYPE_SHADER, T>::value, "getRRender RParticle type static assert");
			_particleContainer.addOrGetRenderer(shaderObj);
		}

		void text()
		{
			SHADER::ShaderMain* testprt;
			getRRender<RENDER::RNormal, SHADER::ShaderMain>(testprt);
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
}

extern RENDER::RenderManager* GRendermanager;