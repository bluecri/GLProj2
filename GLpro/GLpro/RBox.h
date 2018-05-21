#pragma once

#include <list>
#include <utility>
#include <memory>

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
	// Use TextFObj & ShaderText(widthLT, heightLT)

	class RBox
	{
	public:
		using TYPE_SHADER = SHADER::ShaderText;
		using DrawElement = RENDER_TARGET::TEXT::TextFObj*;
		using DrawElemContainer = std::list<DrawElement>;

		RBox(SHADER::ShaderText* shaderObj);
		virtual ~RBox() {};

		// use this with weak_ptr
		DrawElement addToDrawList(DrawElement textFObjBOX);

		void update(CAMERA::Camera* cam);	//shader target camera update

		void draw(float deltaTime);

		virtual void chageShader(SHADER::ShaderText* other);

		virtual SHADER::ShaderText* getShader() const;

		void destructor(DrawElement delElem);

	private:
		SHADER::ShaderText* _shaderObj;

		DrawElemContainer _boxDrawElemContainer;
	};
}