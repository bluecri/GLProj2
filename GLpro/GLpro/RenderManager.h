#pragma once

#include <map>
#include <string>

class RRender;
class RNormal;
class RBox;

namespace RENDER
{
	// Use TextFObj & ShaderText(widthLT, heightLT)
	class RenderManager
	{
	public:
		RenderManager()
		{
			
		}

		
	private:
		RRenderContainerClass<RNormal> a;
		RRenderContainerClass<RBox> b;
	};


	template<class T>	// T = R*****(renderer class)
	class RRenderContainerClass
	{
	public:
		// shader의 구분은 shader file name으로 / renderer의 구분은 shader로
		// shader의 관리는 shader manager / renderer의 관리는 renderer manager
		T* addOrGetRenderer(T::TYPE_SHADER* inShader)
		{
			auto it = _rRenderContainer.find(inShader);
			if (it != _rRenderContainer.end())
			{
				return it.second;
			}

		}
		void removeRenderer(T* delRenderer)
		{
			auto it = _rRenderContainer.find(delRenderer->getShader());
			if (it != _rRenderContainer.end())
			{
				return it.second;
			}
		}

		void changeRenderer

	private:
		std::map<T::TYPE_SHADER*, T*> _rRenderContainer;

	};
}