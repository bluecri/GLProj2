#pragma once

#include <list>
#include <utility>
#include <memory>

namespace CAMERA { class Camera; }

namespace RENDER_TARGET { namespace NORMAL { class NormalFObj; } }
namespace SHADER { class ShaderMain; }

class RigidbodyComponent;

using ROldShadowDrawElement = std::pair<RENDER_TARGET::NORMAL::NormalFObj*, RigidbodyComponent*>;
using ROldShadowDrawElemContainer = std::list<std::shared_ptr<ROldShadowDrawElement>>;

namespace RENDER
{
	class ROldLight
	{
	public:
		ROldLight(SHADER::ShaderMain* shaderObj);
		virtual ~ROldLight() {};

		void update(CAMERA::Camera* cam);	//shader target camera update

		void draw();

		virtual void chageShader(SHADER::ShaderMain* other);

		virtual SHADER::ShaderMain* getShader() const;

		void destructor(std::shared_ptr<ROldShadowDrawElement> shared);
	private:
		SHADER::ShaderMain* _shaderObj;
		CAMERA::Camera *_targetCamera;

		ROldShadowDrawElemContainer _normalDrawElemContainer;
	};
}