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
		using DrawElement = RENDER_TARGET::TEXT::TextFObj;
		using DrawElemContainer = std::list<std::shared_ptr<DrawElement>>;

		RBox(SHADER::ShaderText* shaderObj);
		virtual ~RBox() {};

		std::shared_ptr<DrawElement> addToDrawList(RENDER_TARGET::TEXT::TextFObj* textFObjBOX);

		virtual void updateRRender() override;	//shader target camera update
		void updateTargetCamera(CAMERA::Camera** cam) override;

		virtual void drawRRender(float deltaTime) override;

		void chageShader(SHADER::ShaderText* other);

		SHADER::ShaderText* getShader() const;

	private:
		SHADER::ShaderText* _shaderObj;

		DrawElemContainer _boxDrawElemContainer;
	};
}