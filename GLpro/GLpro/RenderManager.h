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
		// shader�� ������ shader file name���� / renderer�� ������ shader��
		// shader�� ������ shader manager / renderer�� ������ renderer manager
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