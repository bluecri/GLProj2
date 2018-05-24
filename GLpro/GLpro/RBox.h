#pragma once

#include <list>
#include <utility>
#include <memory>

#include "../../RRender.h"

namespace SHADER {
	class ShaderText;
	class ShaderObj;
}
namespace RENDER_TARGET { namespace TEXT { class TextFObj; } }
namespace CAMERA { class Camera; }

class RigidbodyComponent;

namespace RENDER
{
	// Use TextFObj & ShaderText(widthLT, heightLT)

	class RBox : public RRender
	{
	public:
		using TYPE_SHADER = SHADER::ShaderText;
		using DrawElement = RENDER_TARGET::TEXT::TextFObj*;
		using DrawElemContainer = std::list<DrawElement>;

		RBox(SHADER::ShaderText* shaderObj);
		virtual ~RBox() {};

		DrawElement addToDrawList(DrawElement textFObjBOX);

		virtual void update(CAMERA::Camera** cam) override;	//shader target camera update

		virtual void draw(float deltaTime);

		void chageShader(SHADER::ShaderText* other);

		SHADER::ShaderText* getShader() const;

		void destructor(DrawElement delElem);

	private:
		SHADER::ShaderText* _shaderObj;

		DrawElemContainer _boxDrawElemContainer;
	};
}