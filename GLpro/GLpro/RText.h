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
	class RText
	{
	public:
		using TYPE_SHADER = SHADER::ShaderText;
		using RTextDrawElement = std::pair<RENDER_TARGET::TEXT::TextFObj*, RigidbodyComponent*>;
		using RTextDrawElemContainer = std::list<std::shared_ptr<RTextDrawElement>>;

		RText(SHADER::ShaderText* shaderObj);
		virtual ~RText() {};

		// use this with weak_ptr
		std::shared_ptr<RTextDrawElement> addToDrawList(RENDER_TARGET::TEXT::TextFObj* textFObj, RigidbodyComponent* rigidbodyComponent);
				
		void update(CAMERA::Camera* cam);	//shader target camera update

		void draw(float deltaTime);

		virtual void chageShader(SHADER::ShaderText* other);

		virtual SHADER::ShaderText* getShader() const;

		void destructor(std::shared_ptr<RTextDrawElement> shared);

	private:
		SHADER::ShaderText* _shaderObj;
		CAMERA::Camera *_targetCamera;

		RTextDrawElemContainer _textDrawElemContainer;
	};
}