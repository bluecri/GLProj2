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
class Camera;

namespace RENDER
{
	class RText : public RRender
	{
	public:
		using TYPE_SHADER = SHADER::ShaderText;
		using DrawElement = std::pair<RENDER_TARGET::TEXT::TextFObj*, RigidbodyComponent*>;
		using DrawElemContainer = std::list<std::shared_ptr<DrawElement>>;

		RText(SHADER::ShaderText* shaderObj);
		virtual ~RText() {};

		// rigidbody can be nullptr
		std::shared_ptr<DrawElement> addToDrawList(RENDER_TARGET::TEXT::TextFObj* _textFObj, RigidbodyComponent* rigidbodyComponent);
				
		virtual void update(CAMERA::Camera** cam);	//shader target camera update

		virtual void draw(float deltaTime);

		void chageShader(SHADER::ShaderText* other);

		SHADER::ShaderText* getShader() const;

	private:
		SHADER::ShaderText* _shaderObj;

		DrawElemContainer _textDrawElemContainer;
	};
}