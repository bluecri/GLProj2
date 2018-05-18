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

using RBoxDrawElem = RENDER_TARGET::TEXT::TextFObj*;
using RBoxDrawElemContainer = std::list<RBoxDrawElem>;

namespace RENDER
{
	// Use TextFObj & ShaderText(widthLT, heightLT)

	class RBox
	{
	public:
		RBox(SHADER::ShaderText* shaderObj);
		virtual ~RBox() {};

		// use this with weak_ptr
		RBoxDrawElem addToDrawList(RBoxDrawElem textFObjBOX);

		void update(CAMERA::Camera* cam);	//shader target camera update

		void draw(float deltaTime);

		virtual void chageShader(SHADER::ShaderText* other);

		virtual SHADER::ShaderText* getShader() const;

		void destructor(RBoxDrawElem delElem);

	private:
		SHADER::ShaderText* _shaderObj;

		RBoxDrawElemContainer _boxDrawElemContainer;
	};
}